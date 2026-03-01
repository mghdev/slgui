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
    
    int texture_w,texture_h;
    SDL_Texture* content_texture = nullptr;
    Color background_color = COLOR::DARKMODE_BACKGROUND;
    
    View* last_motion_hit = nullptr;
    std::vector<View*> wants_redraw;
    
    void replaceContentTexture(int w, int h);
public:
    SDL_Window* backing_window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::unique_ptr<ViewController> content_vc = nullptr;
    std::shared_ptr<View> content_view = nullptr;  //ownership of the content view can be shared between the window and content_vc
    
    Responder* first_responder = nullptr;
        
    Window(std::unique_ptr<ViewController> vc);
    
    virtual ~Window();
    // delete-ing all of these while the class is WiP
    // when the design is more settled, I will probably implement some of them
    // just very annoying to have to update all the time
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) = delete;
    Window& operator=(Window&& other) = delete;
    // Window(Window&& other) noexcept;
    // Window& operator=(Window&& other) noexcept;
    
    void setBackgroundColor(Color color);
    void setContentVC(std::unique_ptr<ViewController> vc);
    
    void sendEvent(const SDL_Event& event);
    bool makeFirstResponder(Responder& responder);
    
    void viewWantsRedraw(View* v);
    void drawAllIfNeeded();
    void setDirtyRect(Rect r);
    void displayIfNeeded();
    void toggleFullscreen();
    
    void startTextInput();
    void stopTextInput();
};

} // namespace SL




#endif