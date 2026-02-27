#include "SLWindow.hpp"

#include <stdexcept> //runtime_error
#include <format> //format
#include <utility> //exchange, swap

#include "SLGeometry.hpp"

namespace SL {

constexpr Rect DEFAULT_DIRTY_RECT = {0,0,0,0};

Window::Window(std::unique_ptr<ViewController> vc) : first_responder(this)
{
    int window_flags = SDL_WINDOW_OPENGL;
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

Window::Window(Window&& other) noexcept : 
    backing_window(std::exchange(other.backing_window,nullptr)),
    renderer(std::exchange(other.renderer,nullptr))
{}

Window& Window::operator=(Window&& other) noexcept
{
    std::swap(backing_window,other.backing_window);
    std::swap(renderer,other.renderer);
    return *this;
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
        case SDL_EVENT_KEY_DOWN:
            if(event.key.repeat) {
                break;
            }
            if(first_responder) {
                first_responder->keyDown(event);
            }
            break;
        case SDL_EVENT_KEY_UP:
            if(first_responder) {
                first_responder->keyUp(event);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION: {
            float x,y;
            auto state = SDL_GetMouseState(&x,&y);
            if(state == 0) {
                auto v = content_view->hitTest({x,y});
                if(!v) {
                    break;
                }
                v->mouseMoved(event);
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
                    makeFirstResponder(*v);
                    v->leftMouseDown(event);
                    break;
                case SDL_BUTTON_RIGHT:
                    makeFirstResponder(*this);
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
                    makeFirstResponder(*v);
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
    if(first_responder && !first_responder->resignFirstResponder()) {
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

void Window::setDirtyRect(Rect r)
{
    if(dirty_rect == DEFAULT_DIRTY_RECT) {
        dirty_rect = std::move(r);
    }
    else {
        dirty_rect = superRect(dirty_rect,r);
    }
}

void drawAllIfNeeded(SDL_Renderer* renderer, View* v)
{
    v->drawIfNeeded(renderer);
    for(auto& subview : v->subviews) {
        drawAllIfNeeded(renderer,subview.get());
    }
}

void Window::displayIfNeeded()
{
    if(!content_view.get() || dirty_rect == DEFAULT_DIRTY_RECT) {
        return;
    }
    
    drawAllIfNeeded(renderer,content_view.get());
    
    if(!content_texture) {
        content_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,content_view->view_size.x,content_view->view_size.y);
    }
    
    SDL_SetRenderTarget(renderer,content_texture);
    auto r = intersection(dirty_rect,content_view->frame);
    content_view->display(renderer,r,r);
    
    SDL_SetRenderTarget(renderer,NULL);
    SDL_RenderTexture(renderer,content_texture,NULL,NULL);
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

} //namespace SL


