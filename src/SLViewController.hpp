#ifndef SILHOUETTE_INCLUDE_VIEWCONTROLLER_H_
#define SILHOUETTE_INCLUDE_VIEWCONTROLLER_H_

#include <memory>

#include "SLApplication.hpp"
#include "SLBase.hpp"
#include "SLView.hpp"

namespace SL {

class ViewController : public Responder
{
private:
    std::shared_ptr<View> _view;
protected:
    Uint8 dragging_mouse_mask = 0;
public:
    ViewController(std::shared_ptr<View> view);
    ~ViewController();
    
    View& view() {return *_view;};
    
    virtual void respondToEvent(const SDL_Event& event) override;
    
    virtual void mouseDown(const SDL_Event& event);
    virtual void mouseUp(const SDL_Event& event);
    virtual void mouseMoved(const SDL_Event& event);
    virtual void mouseDragged(const SDL_Event& event);
};

} //namespace SL



#endif