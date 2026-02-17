#include "SLWindow.hpp"

#include <stdexcept> //runtime_error
#include <format> //format
#include <utility> //exchange, swap

namespace SL {

Window::Window(std::unique_ptr<ViewController> vc)
{
    int window_flags = SDL_WINDOW_OPENGL;
    if(SDL_CreateWindowAndRenderer(vc->view->frame.w,vc->view->frame.h,window_flags,&backing_window,&renderer) != 0) {
        auto e = SDL_GetError();
        throw std::runtime_error(std::format("Failed to create backing window: {}",e));
    }
    setContentVC(std::move(vc));
}

Window::~Window()
{
    SDL_DestroyWindow(backing_window);
    SDL_DestroyRenderer(renderer);
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
}

void Window::sendEvent(const SDL_Event& event)
{
    if(!content_view) {
        return;
    }
    switch (event.type)
    {
        case SDL_WINDOWEVENT:
            break;
        case SDL_KEYDOWN:
            if(event.key.repeat) {
                break;
            }
            if(first_responder) {
                first_responder->keyDown(event);
            }
            break;
        case SDL_KEYUP:
            if(first_responder) {
                first_responder->keyUp(event);
            }
            break;
        case SDL_MOUSEMOTION: {
            int x,y;
            auto state = SDL_GetMouseState(&x,&y);
            if(state == 0) {
                auto v = content_view->hitTest(PointCast<double,int>({x,y}));
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
        case SDL_MOUSEBUTTONDOWN: {
            auto v = content_view->hitTest(PointCast<double,int>({event.button.x,event.button.y}));
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
        case SDL_MOUSEBUTTONUP: {
            auto v = content_view->hitTest(PointCast<double,int>({event.button.x,event.button.y}));
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
        case SDL_MOUSEWHEEL: {
            int x,y;
            SDL_GetMouseState(&x,&y);
            auto v = content_view->hitTest(PointCast<double,int>({x,y}));
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

void Window::displayIfNeeded()
{
    SDL_SetRenderDrawColor(renderer,BLACK.r,BLACK.g,BLACK.b,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    content_view->draw(renderer,content_view->frame,content_view->frame);
    SDL_RenderPresent(renderer);
}

void Window::toggleFullscreen()
{
    if (!in_fullscreen) {
        SDL_GetWindowPosition(backing_window,&last_windowed_rect.x,&last_windowed_rect.y);
        SDL_GetWindowSize(backing_window,&last_windowed_rect.w,&last_windowed_rect.h);
        SDL_SetWindowFullscreen(backing_window,SDL_WINDOW_FULLSCREEN_DESKTOP);
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


