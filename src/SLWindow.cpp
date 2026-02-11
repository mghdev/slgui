#include "SLWindow.hpp"

#include <stdexcept> //runtime_error
#include <format> //format
#include <utility> //exchange, swap

namespace SL {

Window::Window(int width, int height)
{
    int window_flags = SDL_WINDOW_OPENGL;
    if(SDL_CreateWindowAndRenderer(width,height,window_flags,&backing_window,&renderer) != 0) {
        auto e = SDL_GetError();
        throw std::runtime_error(std::format("Failed to create backing window: {}",e));
    }
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

void Window::sendEvent(const SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_WINDOWEVENT:
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if(first_responder) {
                first_responder->respondToEvent(event);
            }
            else {
                respondToEvent(event);
            }
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            auto v = content_view->hitTest({event.button.x,event.button.y});
            
            break;
        }
        case SDL_MOUSEWHEEL:
            break;
        default:
            break;
    }
}

void Window::displayIfNeeded()
{
    SDL_SetRenderDrawColor(renderer,BLACK.r,BLACK.g,BLACK.b,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    if (content_vc) {
        content_vc->view().draw(renderer);
    }
    else if(content_view) {
        content_view->draw(renderer);
    }
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


