#ifndef SILHOUETTE_INCLUDE_VIEW_H_
#define SILHOUETTE_INCLUDE_VIEW_H_

#include <vector>
#include <memory>

#include <SDL.h>

#include "SLBase.hpp"
#include "SLEvent.hpp"

namespace SL {

class Window;
class View : public Responder
{
protected:
    bool needs_display = true;
public:
    Window* window = nullptr;
    View* superview = nullptr;
    Rect frame; // Placement within superview's coordinate system (root view's frame is the same as the window's rect)
    Rect bounds;  // Rect within own coordinate system that is currently aligned with frame.
    
    std::vector<std::shared_ptr<View>> subviews; //ownership of subviews can be shared between superview and view controller
    
    bool is_hidden = false;
    double z_position = 0.0f;
    Color background_color = BLACK;
    
    View(Rect rect);
    
    virtual void setFrame(Rect r);
    virtual View* hitTest(Point p);
    virtual Rect transformRectFrom(Rect r, const View* other) const;
    virtual Rect transformRectTo(Rect r, const View* other) const;
    
    virtual void setWindow(Window* window);
    virtual void addSubview(std::shared_ptr<View> v);
    virtual void removeFromSuperview();
    
    virtual bool isDescendantOf(const View& other) const;
    virtual View* closestSharedAncestor(const View& other);
    
    // The main draw call does some rect transforms then calls, in order: drawBackground, drawContent, drawSubviews
    // The intent is for subclasses of SL::View to override drawContent to perform custom drawing.
    virtual void draw(SDL_Renderer* renderer, const Rect& visible_frame, const Rect& window_coords);
    virtual void drawBackground(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords);
    virtual void drawContent(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords);
    virtual void drawSubviews(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords);
};

} //namespace SL



#endif