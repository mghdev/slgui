#ifndef SILHOUETTE_INCLUDE_VIEW_H_
#define SILHOUETTE_INCLUDE_VIEW_H_

#include <vector>
#include <memory>

#include <SDL3/SDL.h>

#include "SLBase.hpp"
#include "SLEvent.hpp"

namespace SL {

SDL_FRect toSDLRect(Rect r);

class Window;
class View : public Responder
{
protected:    
    typedef enum Flags {
        NONE = 0x00,
        NEEDS_REDRAW = 0x01,
        IS_HIDDEN = 0x02
    } Flags;
    int8_t flags;
    
    float border_width = 0;
    Color border_color = COLOR::NONE;
    Color background_color = COLOR::NONE;
    SDL_Texture * texture = nullptr;
public:
    Window* window = nullptr;
    View* superview = nullptr;
    
    // Placement within superview's coordinate system (root view's frame is the same as the window's rect)
    Rect frame;
    // Rect within own coordinate system that is currently aligned with frame.
    Rect bounds;  
    
    //Size of the view's content in its internal coordinate system
    //This is fixed on view construction
    Vec2F const view_size;  
    
    // ownership of subviews can be shared between superview and view controller
    std::vector<std::shared_ptr<View>> subviews; 
    
    View(Vec2F size);
    
    virtual ~View();
    // delete-ing all of these while the class is WiP
    // when the design is more settled, I will probably implement some of them
    // just very annoying to have to update all the time
    View(const View&) = delete;
    View& operator=(const View&) = delete;
    View(View&& other) = delete;
    // View(View&& other) noexcept;
    View& operator=(View&& other) = delete;
    
    virtual void requestDisplay();
    virtual void setFrame(Rect r);
    virtual void setBounds(Rect r);
    virtual View* hitTest(Point p);
    virtual Rect transformRectFrom(Rect r, const View* other) const;
    virtual Rect transformRectTo(Rect r, const View* other) const;
    
    virtual void setWindow(Window* window);
    virtual void addSubview(std::shared_ptr<View> v);
    virtual void removeFromSuperview();
    virtual bool isDescendantOf(const View& other) const;
    virtual View* closestSharedAncestor(const View& other);
    
    virtual void setBackgroundColor(Color color);
    virtual void setBorderWidth(decltype(border_width) width);
    virtual void setBorderColor(Color color);
    virtual void setBorderWidthAndColor(decltype(border_width) width, Color color);
    
    // The main draw call does some rect transforms then calls, in order: drawBackground, drawContent, drawSubviews
    // The intent is for subclasses of SL::View to override drawContent to perform custom drawing.
    virtual void drawIfNeeded(SDL_Renderer* renderer);
    virtual void drawBackground(SDL_Renderer* renderer);
    virtual void drawBorder(SDL_Renderer* renderer);
    virtual void drawContent(SDL_Renderer* renderer);
    
    virtual void display(SDL_Renderer* renderer, const Rect& target_frame, const Rect& window_coords);
    virtual void displaySubviews(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords);
};

} //namespace SL



#endif