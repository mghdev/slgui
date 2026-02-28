#include "SLView.hpp"

#include <algorithm> //erase_if
#include <utility> //exchange

#include "SLGeometry.hpp"
#include "SLWindow.hpp"

namespace SL {

Rect transformToSuperview(Rect r, const View& v)
{
    auto x_scale = v.frame.w / v.bounds.w;
    auto y_scale = v.frame.h / v.bounds.h;
    auto x_offset = v.bounds.x * x_scale + v.frame.x;
    auto y_offset = v.bounds.y * y_scale + v.frame.y;
    return r.transform({x_offset,y_offset},{x_scale,y_scale});
}

SDL_FRect toSDLRect(Rect r)
{
    return SDL_FRect(r.x,r.y,r.w,r.h);
}

View::View(Vec2F size) : 
    frame(0,0,size.x,size.y), bounds(0,0,size.x,size.y), view_size(size)
{
    
}

View::~View()
{
    SDL_DestroyTexture(texture);
}

View::View(View&& other) noexcept :
    view_size(other.view_size),
    texture(std::exchange(other.texture,nullptr)),
    window(             std::move(other.window)),
    superview(          std::move(other.superview)),
    frame(              std::move(other.frame)),
    bounds(             std::move(other.bounds)),
    subviews(           std::move(other.subviews)),
    needs_redraw(       std::move(other.needs_redraw)),
    is_hidden(          std::move(other.is_hidden)),
    background_color(   std::move(other.background_color))
{
    
}

void View::requestDisplay()
{
    needs_redraw = true;
    if(window) {
        auto r = transformRectTo(bounds,nullptr);
        window->setDirtyRect(r);
    }
}

void View::setFrame(Rect r) 
{
    if(window) {
        // we don't need to re-draw the view's content
        // so just update the window's dirty rect without setting this->needs_redraw
        auto dirty = superRect(r,frame);
        if(superview) {
            dirty = superview->transformRectTo(dirty,nullptr);
        }
        window->setDirtyRect(dirty);
    }
    
    // scale the bounds so that relative size stays fixed
    auto x_scale = r.w/frame.w;
    auto y_scale = r.h/frame.h;
    bounds = {bounds.x,bounds.y,bounds.w*x_scale,bounds.h*y_scale};
    
    frame=r;
}

void View::setBounds(Rect r)
{
    if(window) {
        window->setDirtyRect(transformRectTo(bounds,nullptr));
    }
    bounds = r;
}


View* View::hitTest(Point p)
{
    if(isPointInRect(p,frame)) {
        auto x_scale = bounds.w / frame.w;
        auto y_scale = bounds.h / frame.h;
        auto p2 = Point{(p.x-frame.x)*x_scale,(p.y-frame.y)*y_scale};
        for(auto& sub : subviews) {
            auto v = sub->hitTest(p2);
            if(v) {
                return v;
            }
        }
        return this;
    }
    return nullptr;
}

Rect View::transformRectFrom(Rect r, const View* other) const
{
    if(other) {
        return other->transformRectTo(r,this);
    }
    
    auto reverse = transformRectTo({0,0,1,1},nullptr);
    r = {
        (r.x-reverse.x)/reverse.w,
        (r.y-reverse.y)/reverse.h,
        r.w/reverse.w,
        r.h/reverse.h
    };
    return r;
}

Rect View::transformRectTo(Rect r, const View* other) const
{
    // assume this->window is non-null, otherwise it is an error to call this method
    if(other) {
        // also assume other->window == this->window
        // convert to other view's coordinate system
        auto v = this;
        while(!(v == other || other->isDescendantOf(*v))) {
            r = transformToSuperview(r,*v);
            v = v->superview;
        }
        if(v != other) {
            auto reverse = other->transformRectTo({0,0,1,1},v);
            r = {
                (r.x-reverse.x)/reverse.w,
                (r.y-reverse.y)/reverse.h,
                r.w/reverse.w,
                r.h/reverse.h
            };
        }
    }
    else {
        // otherwise, convert to window coordinates
        auto v = this;
        while(v) {
            r = transformToSuperview(r,*v);
            v = v->superview;
        }
    }
    return r;
}

void View::setWindow(Window* w)
{
    window = w;
    if(texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    requestDisplay();
    
    for(auto& view : subviews) {
        view->setWindow(w);
    }
}

void View::addSubview(std::shared_ptr<View> v)
{
    v->next_responder = this;
    v->superview = this;
    subviews.push_back(v);
}

void View::removeFromSuperview()
{
    if(superview) {
        std::erase_if(superview->subviews,
            [&](auto val) {
                return val.get() == this;
            }
        );
    }
    superview = nullptr;
}

bool View::isDescendantOf(const View& other) const
{
    if(superview) {
        return superview == &other ? true : superview->isDescendantOf(other);
    }
    return false;
}

View* View::closestSharedAncestor(const View& other)
{
    if(this == &other || other.isDescendantOf(*this)) {
        return this;
    }
    if(superview) {
        return superview->closestSharedAncestor(other);
    }
    return nullptr;
}

void View::drawIfNeeded(SDL_Renderer* renderer)
{
    if(!needs_redraw) {
        return;
    }
    
    if(!texture) {
        texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,view_size.x,view_size.y);
    }
    
    SDL_SetRenderTarget(renderer,texture);
    drawBackground(renderer);
    drawContent(renderer);
    needs_redraw = false;
}

void View::drawBackground(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    SDL_RenderClear(renderer);
}

void View::drawContent(SDL_Renderer* renderer)
{
    // subclasses of SL::View can override this method to do custom drawing
}

void View::display(SDL_Renderer* renderer, const Rect& target_frame, const Rect& window_coords)
{
    if(is_hidden || target_frame.w <= 0 || target_frame.h <= 0) {
        return;
    }
    
    // convert target_frame to internal coordinate system
    auto x_scale = bounds.w/frame.w;
    auto y_scale = bounds.h/frame.h;
    auto visible_bounds = Rect{
        (target_frame.x-frame.x)*x_scale,
        (target_frame.y-frame.y)*y_scale,
        target_frame.w*x_scale,
        target_frame.h*y_scale
    };
    
    // size of content is fixed at this->view_size, but visible_bounds may extend farther
    auto left = clamp(0.0,visible_bounds.x,view_size.x);
    auto right = clamp(0.0,visible_bounds.x+visible_bounds.w,view_size.x);
    auto top = clamp(0.0,visible_bounds.y,view_size.y);
    auto bottom = clamp(0.0,visible_bounds.y+visible_bounds.h,view_size.y);
    
    auto src = Rect{left,top,right-left,bottom-top};
    auto dst = transformRectTo(src,nullptr);
    
    auto sdl_src = toSDLRect(src);
    auto sdl_dst = toSDLRect(dst);
    
    SDL_RenderTexture(renderer,texture,&sdl_src,&sdl_dst);
    
    displaySubviews(renderer,visible_bounds,window_coords);
}

void View::displaySubviews(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords)
{
    for(auto& subview : subviews) {
        auto visible_rect = intersection(visible_bounds,subview->frame);
        auto subview_window_coords = transformRectTo(visible_rect,nullptr);
        subview->display(renderer,visible_rect,subview_window_coords);
    }
}


} //namespace SL

