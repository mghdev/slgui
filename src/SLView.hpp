#ifndef SILHOUETTE_INCLUDE_VIEW_H_
#define SILHOUETTE_INCLUDE_VIEW_H_

#include <vector>
#include <memory>

#include <SDL.h>

#include "SLBase.hpp"
#include "SLApplication.hpp"

namespace SL {

class View : public Responder
{
protected:
    bool needs_display = true;
public:
    Window* window;
    View* superview;
    Rect frame; // Placement within superview's coordinate system
    Rect bounds;  // Rect within own coordinate system that is currently aligned with frame.
    
    std::vector<std::shared_ptr<View>> subviews; //ownership of subviews can be shared between superview and view controller
    float z_position = 0.0f;
    
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
    
    virtual void draw(SDL_Renderer* renderer, const Point& offset = {0,0});
};

} //namespace SL



#endif