#ifndef SILHOUETTE_INCLUDE_EVENT_H_
#define SILHOUETTE_INCLUDE_EVENT_H_

#include <SDL3/SDL.h>

namespace SL {

class Responder
{
protected:
public:
    Responder* next_responder = nullptr;
    
    virtual bool acceptsFirstResponder();
    virtual bool becomeFirstResponder();
    virtual bool resignFirstResponder();
    
    virtual void leftMouseDown(const SDL_Event& event);
    virtual void rightMouseDown(const SDL_Event& event);
    virtual void otherMouseDown(const SDL_Event& event);
    
    virtual void leftMouseUp(const SDL_Event& event);
    virtual void rightMouseUp(const SDL_Event& event);
    virtual void otherMouseUp(const SDL_Event& event);
    
    virtual void mouseMoved(const SDL_Event& event);
    virtual void mouseEntered(const SDL_Event& event);
    virtual void mouseExited(const SDL_Event& event);
    
    virtual void leftMouseDragged(const SDL_Event& event);
    virtual void rightMouseDragged(const SDL_Event& event);
    virtual void otherMouseDragged(const SDL_Event& event);
    
    virtual void scrollWheel(const SDL_Event& event);
    
    virtual void keyDown(const SDL_Event& event);
    virtual void keyUp(const SDL_Event& event);
    virtual void keyHold(const SDL_Event& event);
    
    virtual void textInput(const SDL_Event& event);
};

} //namespace SL



#endif