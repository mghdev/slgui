#ifndef SILHOUETTE_INCLUDE_WINDOW_H_
#define SILHOUETTE_INCLUDE_WINDOW_H_

#include <memory>
#include <SDL3/SDL.h>

#include "SLBase.hpp"
#include "SLEvent.hpp"
#include "SLViewController.hpp"
#include "SLView.hpp"

namespace SL {

class Window : public Responder
{
protected:
    bool in_fullscreen = false;
    RectI last_windowed_rect;
    Rect dirty_rect;
public:
    SDL_Window* backing_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::unique_ptr<ViewController> content_vc = nullptr;
    std::shared_ptr<View> content_view = nullptr;  //ownership of the content view can be shared between the window and content_vc
    
    Responder* first_responder = nullptr;
        
    Window(std::unique_ptr<ViewController> vc);
    
    virtual ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;
    
    void setContentVC(std::unique_ptr<ViewController> vc);
    
    void sendEvent(const SDL_Event& event);
    bool makeFirstResponder(Responder& responder);
    
    void setDirtyRect(Rect r);
    void displayIfNeeded();
    void toggleFullscreen();
};

} // namespace SL




#endif