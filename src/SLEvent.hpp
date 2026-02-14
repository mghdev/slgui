#ifndef SILHOUETTE_INCLUDE_EVENT_H_
#define SILHOUETTE_INCLUDE_EVENT_H_

#include <SDL.h>

namespace SL {

class Responder
{
protected:
public:
    Responder* next_responder = nullptr;
    
    virtual bool acceptsFirstResponder();
    virtual bool becomeFirstResponder();
    virtual bool resignFirstResponder();
    
    virtual void respondToEvent(const SDL_Event& e);
    
    virtual void leftMouseDown(const SDL_Event& e);
    virtual void rightMouseDown(const SDL_Event& e);
    virtual void otherMouseDown(const SDL_Event& e);
    
    virtual void leftMouseUp(const SDL_Event& e);
    virtual void rightMouseUp(const SDL_Event& e);
    virtual void otherMouseUp(const SDL_Event& e);
    
    virtual void mouseMoved(const SDL_Event& e);
    
    virtual void leftMouseDragged(const SDL_Event& e);
    virtual void rightMouseDragged(const SDL_Event& e);
    virtual void otherMouseDragged(const SDL_Event& e);
    
    virtual void scrollWheel(const SDL_Event& e);
    
    virtual void keyDown(const SDL_Event& e);
    virtual void keyUp(const SDL_Event& e);
};

} //namespace SL



#endif