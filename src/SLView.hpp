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
    Rect _rect;
    Rect visible_rect;
    View* _superview;
    Window* _window;
    bool _needs_display = true;
public:
    // std::vector<View*> const& subviews = _subviews;

    Rect const& rect = _rect;
    View* const& superview = _superview;
    bool const& needs_display = _needs_display;
    
    std::vector<std::shared_ptr<View>> subviews;
    float z_position = 0.0f;
    
    View(Rect rect);
    
    virtual void draw(SDL_Renderer* renderer, const Point& offset = {0,0});
    
    virtual void setRect(Rect r);
    virtual View* hitTest(Point p);
    virtual Rect convertRectFrom(Rect r, const View& other) const;
    virtual Rect convertRectTo(Rect r, const View& other) const;
    
    virtual void addSubview(std::shared_ptr<View> v);
    virtual void removeFromSuperview();
    
    virtual bool isDescendantOf(const View& other) const;
    virtual View* closestSharedAncestor(const View& other) const;
};

} //namespace SL



#endif