#include "SLWindow.hpp"

#include <stdexcept> //runtime_error
#include <format> //format
#include <utility> //exchange, swap

#include "SLGeometry.hpp"

namespace SL {

constexpr Rect DEFAULT_DIRTY_RECT = {0,0,0,0};

Window::Window(std::unique_ptr<ViewController> vc) : first_responder(this)
{
    int window_flags = 0;
    auto success = SDL_CreateWindowAndRenderer( "",
                                                vc->view->frame.w,
                                                vc->view->frame.h,
                                                window_flags,
                                                &backing_window,
                                                &renderer);
    
    if(!success) {
        auto e = SDL_GetError();
        throw std::runtime_error(std::format("Failed to create backing window: {}",e));
    }
    setContentVC(std::move(vc));
    setDirtyRect(content_view->frame);
}

Window::~Window()
{
    SDL_DestroyWindow(backing_window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(content_texture);
}

// TODO: bring these back when design is more stable
// Window::Window(Window&& other) noexcept :
//     backing_window( std::exchange(other.backing_window,nullptr)),
//     renderer(       std::exchange(other.renderer,nullptr)),
//     content_texture(std::exchange(other.content_texture,nullptr)),
//     in_fullscreen(      std::move(other.in_fullscreen)),
//     last_windowed_rect( std::move(other.last_windowed_rect)),
//     dirty_rect(         std::move(other.dirty_rect)),
//     texture_w(          std::move(other.texture_w)),
//     texture_h(          std::move(other.texture_h)),
//     background_color(   std::move(other.background_color)),
//     content_vc(         std::move(other.content_vc)),
//     content_view(       std::move(other.content_view)),
//     first_responder(    std::move(other.first_responder))
// {}

// Window& Window::operator=(Window&& other) noexcept
// {
//     std::swap(backing_window,other.backing_window);
//     std::swap(renderer,other.renderer);
//     std::swap(content_texture,other.content_texture);
    
//     in_fullscreen       = std::move(other.in_fullscreen);
//     last_windowed_rect  = std::move(other.last_windowed_rect);
//     dirty_rect          = std::move(other.dirty_rect);
//     texture_w           = std::move(other.texture_w);
//     texture_h           = std::move(other.texture_h);
//     background_color    = std::move(other.background_color);
//     content_vc          = std::move(other.content_vc);
//     content_view        = std::move(other.content_view);
//     first_responder     = std::move(other.first_responder);
    
//     return *this;
// }

void Window::setBackgroundColor(Color color)
{
    background_color = color;
    setDirtyRect(content_view->frame);
}

void Window::setContentVC(std::unique_ptr<ViewController> vc)
{
    content_vc = std::move(vc);
    content_view = content_vc->view;
    content_vc->next_responder = this;
    content_vc->view->setWindow(this);
    SDL_DestroyTexture(content_texture);
    content_texture = nullptr;
}

void Window::sendEvent(const SDL_Event& event)
{
    if(!content_view) {
        return;
    }
    switch (event.type)
    {
        case SDL_EVENT_WINDOW_RESIZED: {
            auto w = event.window.data1;
            auto h = event.window.data2;
            if(w > texture_w || h > texture_h) {
                replaceContentTexture(std::max(w,texture_w),std::max(h,texture_h));
            }
            content_view->setFrame(Rect(0,0,w,h));
            break;
        }
        case SDL_EVENT_TEXT_INPUT:
            first_responder->textInput(event);
            break;
        case SDL_EVENT_KEY_DOWN:
            if(event.key.repeat) {
                first_responder->keyHold(event);
            }
            else {
                first_responder->keyDown(event);
            }
            break;
        case SDL_EVENT_KEY_UP:
            first_responder->keyUp(event);
            break;
        case SDL_EVENT_MOUSE_MOTION: {
            float x,y;
            auto state = SDL_GetMouseState(&x,&y);
            auto v = content_view->hitTest({x,y});
            if(v != last_motion_hit) {
                if(last_motion_hit) {
                    last_motion_hit->mouseExited(event);
                }
                if(v) {
                    v->mouseEntered(event);
                }
                last_motion_hit = v;
            }
            if(state == 0) {
                if(v) {
                    v->mouseMoved(event);
                }
            }
            else {
                if(state & SDL_BUTTON_LMASK) {
                    first_responder->leftMouseDragged(event);
                }
                if(state & SDL_BUTTON_RMASK) {
                    first_responder->rightMouseDragged(event);
                }
                if(state & ~(SDL_BUTTON_LMASK | SDL_BUTTON_RMASK)) {
                    first_responder->otherMouseDragged(event);
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            auto v = content_view->hitTest({event.button.x,event.button.y});
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    if(v->acceptsFirstResponder()) {
                        makeFirstResponder(*v);
                    }
                    v->leftMouseDown(event);
                    break;
                case SDL_BUTTON_RIGHT:
                    if(v->acceptsFirstResponder()) {
                        makeFirstResponder(*this);
                    }
                    v->rightMouseDown(event);
                    break;
                default:
                    v->otherMouseDown(event);
                    break;
            }
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            auto v = content_view->hitTest({event.button.x,event.button.y});
            if(!v) {
                break;
            }
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    // first responder status changes on left mouse down, not up
                    v->leftMouseUp(event);
                    break;
                case SDL_BUTTON_RIGHT:
                    first_responder->rightMouseUp(event);
                    break;
                default:
                    if(v->acceptsFirstResponder()) {
                        makeFirstResponder(*v);
                    }
                    v->otherMouseUp(event);
                    break;
            }
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            float x,y;
            SDL_GetMouseState(&x,&y);
            auto v = content_view->hitTest({x,y});
            if(!v) {
                break;
            }
            v->scrollWheel(event);
            break;
        }
        default:
            break;
    }
}

bool Window::makeFirstResponder(Responder& responder)
{
    if(!first_responder->resignFirstResponder()) {
        return false;
    }
    if(responder.becomeFirstResponder()) {
        first_responder = &responder;
    }
    else {
        first_responder = this;
    }
    return true;
}

void Window::viewWantsRedraw(View* v)
{
    wants_redraw.push_back(v);
}

void Window::setDirtyRect(Rect r)
{
    if(dirty_rect == DEFAULT_DIRTY_RECT) {
        dirty_rect = std::move(r);
    }
    else {
        dirty_rect = superRect(dirty_rect,r);
    }
}

void Window::drawAllIfNeeded()
{
    while(wants_redraw.size() > 0) {
        auto v = wants_redraw.back();
        v->drawIfNeeded(renderer);
        wants_redraw.pop_back();
    }
}

void Window::replaceContentTexture(int w, int h)
{
    if(content_texture) {
        SDL_DestroyTexture(content_texture);
    }
    texture_w = w;
    texture_h = h;
    content_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,w,h);
}

void Window::displayIfNeeded()
{
    if(!content_view.get() || dirty_rect == DEFAULT_DIRTY_RECT) {
        return;
    }
    
    drawAllIfNeeded();
    
    if(!content_texture) {
        // allocate the size we would need for 1:1 fullscreen pixels
        int display_idx = SDL_GetDisplayForWindow(backing_window);
        auto mode = SDL_GetDesktopDisplayMode(display_idx);
        replaceContentTexture(mode->w,mode->h);
    }
    
    SDL_SetRenderTarget(renderer,content_texture);
    auto r = intersection(dirty_rect,content_view->frame);
    
    SDL_SetRenderDrawColor(renderer,background_color.r,background_color.g,background_color.b,background_color.a);
    auto sdl_r = toSDLRect(r);
    SDL_RenderFillRect(renderer,&sdl_r); //background
    content_view->display(renderer,r,r);
    
    SDL_SetRenderTarget(renderer,NULL);
    auto window_sdl_r = toSDLRect(content_view->frame);
    SDL_RenderTexture(renderer,content_texture,&window_sdl_r,&window_sdl_r);
    SDL_RenderPresent(renderer);
    
    dirty_rect = DEFAULT_DIRTY_RECT;
}

void Window::toggleFullscreen()
{
    if (!in_fullscreen) {
        SDL_GetWindowPosition(backing_window,&last_windowed_rect.x,&last_windowed_rect.y);
        SDL_GetWindowSize(backing_window,&last_windowed_rect.w,&last_windowed_rect.h);
        SDL_SetWindowFullscreen(backing_window,SDL_WINDOW_FULLSCREEN);
        in_fullscreen = true;
    }
    else {
        SDL_SetWindowFullscreen(backing_window,0);
        SDL_SetWindowPosition(backing_window,last_windowed_rect.x,last_windowed_rect.y);
        SDL_SetWindowSize(backing_window,last_windowed_rect.w,last_windowed_rect.h);
        in_fullscreen = false;
    }
}

void Window::startTextInput()
{
    SDL_StartTextInput(backing_window);
}

void Window::stopTextInput()
{
    SDL_StopTextInput(backing_window);
}


} //namespace SL


