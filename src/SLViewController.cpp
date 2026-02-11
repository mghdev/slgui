#include "SLViewController.hpp"

#include "SLGeometry.hpp"

namespace SL {

ViewController::ViewController(std::shared_ptr<View> view) : _view(view)
{
    // event_system.addEventWatch(SDL_MOUSEBUTTONDOWN,this);
    // event_system.addEventWatch(SDL_MOUSEBUTTONUP,this);
    // event_system.addEventWatch(SDL_MOUSEMOTION,this);
    // event_system.addEventWatch(SDL_MOUSEWHEEL,this);
}

ViewController::~ViewController()
{
}

void ViewController::respondToEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN: {
        Point loc{event.button.x,event.button.y};
        if(isPointInRect(loc,_view->rect)) {
            mouseDown(event);
        }
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        Point loc{event.button.x,event.button.y};
        if(isPointInRect(loc,_view->rect)) {
            mouseUp(event);
        }
        break;
    }
    case SDL_MOUSEMOTION: {
        Point loc{event.motion.x,event.motion.y};
        if(isPointInRect(loc,_view->rect)) {
            if(event.motion.state) {
                mouseDragged(event);
            }
            else {
                mouseMoved(event);
            }
        }
        break;
    }
    default:
        break;
    }
}

void ViewController::mouseDown(const SDL_Event& event)
{
     
}

void ViewController::mouseUp(const SDL_Event& event)
{
    
}

void ViewController::mouseDragged(const SDL_Event& event)
{
    
}

void ViewController::mouseMoved(const SDL_Event& event)
{
    
}

} //namespace SL



