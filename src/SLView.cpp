#include "SLView.hpp"

#include <algorithm> //erase_if
// #include <stdexcept> //runtime_error, invalid_argument

#include "SLGeometry.hpp"
#include "SLWindow.hpp"

namespace SL {

View::View(Rect rect) : 
    frame(rect), bounds(0,0,rect.w,rect.h)
{
    
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

Rect transformToSuperview(Rect r, const View& v)
{
    auto x_scale = v.frame.w / v.bounds.w;
    auto y_scale = v.frame.h / v.bounds.h;
    auto x_offset = v.bounds.x * x_scale + v.frame.x;
    auto y_offset = v.bounds.y * y_scale + v.frame.y;
    return r.transform({x_offset,y_offset},{x_scale,y_scale});
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


void View::setFrame(Rect r) 
{
    frame=r;
    needs_display = true;
}

void View::setWindow(Window* w)
{
    window = w;
    for(auto& view : subviews) {
        view->setWindow(w);
    }
}

void View::addSubview(std::shared_ptr<View> v)
{
    v->next_responder = this;
    v->superview = this;
    needs_display = needs_display | v->needs_display;
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

void View::draw(SDL_Renderer* renderer, const Rect& visible_frame, const Rect& window_coords)
{
    if(is_hidden || visible_frame.w <= 0 || visible_frame.h <= 0) {
        return;
    }
    
    auto x_scale = bounds.w/frame.w;
    auto y_scale = bounds.h/frame.h;
    auto visible_bounds = Rect{
        (visible_frame.x-frame.x)*x_scale,
        (visible_frame.y-frame.y)*y_scale,
        visible_frame.w*x_scale,
        visible_frame.h*y_scale
    };
    drawBackground(renderer,visible_bounds,window_coords);
    drawContent(renderer,visible_bounds,window_coords);
    drawSubviews(renderer,visible_bounds,window_coords);
    needs_display = false;
}

void View::drawBackground(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords)
{
    Color saved;
    SDL_GetRenderDrawColor(renderer,&saved.r,&saved.g,&saved.b,&saved.a);
    
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    auto sdl = SDL_FRect(window_coords.x,window_coords.y,window_coords.w,window_coords.h);
    SDL_RenderFillRect(renderer,&sdl);
    
    SDL_SetRenderDrawColor(renderer,saved.r,saved.g,saved.b,saved.a);
}

void View::drawContent(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords)
{
    // subclasses of SL::View can override this method to do custom drawing
}

void View::drawSubviews(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords)
{
    for(auto& subview : subviews) {
        auto visible_rect = Rect{
            subview->frame.x,
            subview->frame.y,
            std::min(subview->frame.x+subview->frame.w,visible_bounds.x+visible_bounds.w)-subview->frame.x,
            std::min(subview->frame.y+subview->frame.h,visible_bounds.y+visible_bounds.h)-subview->frame.y
        };
        auto subview_window_coords = transformRectTo(visible_rect,nullptr);
        subview->draw(renderer,visible_rect,subview_window_coords);
    }
}


} //namespace SL

