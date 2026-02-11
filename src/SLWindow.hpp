#ifndef SILHOUETTE_INCLUDE_WINDOW_H_
#define SILHOUETTE_INCLUDE_WINDOW_H_

#include <SDL.h>
#include "SLBase.hpp"
#include "SLApplication.hpp"
#include "SLViewController.hpp"
#include "SLView.hpp"

namespace SL {

class Window : public Responder
{
protected:
    bool in_fullscreen = false;
    Rect last_windowed_rect;
public:
    SDL_Window* backing_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    ViewController* content_vc = nullptr;
    View* content_view = nullptr;
    View* first_responder = nullptr;
    
    Window(int width, int height);
    
    ~Window();
    Window(const Window&) = delete; //copy ctor
    Window& operator=(const Window&) = delete; //copy assign
    Window(Window&& other) noexcept; //move ctor
    Window& operator=(Window&& other) noexcept; //move assign
    
    void sendEvent(const SDL_Event& event);
    
    void displayIfNeeded();
    void toggleFullscreen();
};

} // namespace SL




#endif