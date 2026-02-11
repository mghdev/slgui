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
    
    virtual View* hitTest(Point p);
    virtual void setRect(Rect r);
    virtual void addSubview(std::shared_ptr<View> v);
    virtual void removeFromSuperview();
};

} //namespace SL



#endif