#include "SLView.hpp"

#include <algorithm> //erase_if

#include "SLGeometry.hpp"
#include "SLWindow.hpp"

namespace SL {

View::View(Rect rect) : 
    rect(rect) 
{
    
}

void View::draw(SDL_Renderer* renderer, const Point& offset)
{
    for(auto& subview : subviews) {
        subview->draw(renderer,rect.origin()+offset);
    }
    _needs_display = false;
}

View* View::hitTest(Point p)
{
    if(isPointInRect(p,rect)) {
        for(auto& sub : subviews) {
            auto v = sub->hitTest(p-origin(sub->rect));
            if(v) {
                return v;
            }
        }
        return this;
    }
    return nullptr;
}

void View::setRect(Rect r) 
{
    _rect=r;
    _needs_display = true;
}

void View::addSubview(std::shared_ptr<View> v)
{
    v->setNextResponder(this);
    _needs_display = v->needs_display;
    subviews.push_back(v);
}

void View::removeFromSuperview()
{
    if(_superview) {
        std::erase_if(_superview->subviews,
            [&](auto val) {
                return val.get() == this;
            }
        );
    }
}

} //namespace SL

