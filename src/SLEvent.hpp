#ifndef SILHOUETTE_INCLUDE_EVENT_H_
#define SILHOUETTE_INCLUDE_EVENT_H_

#include <SDL.h>

namespace SL {

// typedef enum {
//     KEYDOWN = SDL_KEYDOWN,
//     KEYUP = SDL_KEYUP,
//     MOUSEMOTION = SDL_MOUSEMOTION,
//     MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN,
//     MOUSEBUTTONUP = SDL_MOUSEBUTTONUP,
//     MOUSEWHEEL = SDL_MOUSEWHEEL
// } EventType;

// struct Event
// {
//     SDL_Event raw_event;
    
//     EventType type() const;
// };

class Responder
{
protected:
    Responder* next_responder = nullptr;
public:
    virtual void setNextResponder(Responder* r);
    
    virtual void respondToEvent(const SDL_Event& e);
    
    virtual void leftMouseDown(const SDL_Event& e);
    virtual void rightMouseDown(const SDL_Event& e);
    virtual void otherMouseDown(const SDL_Event& e);
    
    virtual void leftMouseUp(const SDL_Event& e);
    virtual void rightMouseUp(const SDL_Event& e);
    virtual void otherMouseUp(const SDL_Event& e);
    
    virtual void leftMouseDragged(const SDL_Event& e);
    virtual void rightMouseDragged(const SDL_Event& e);
    virtual void otherMouseDragged(const SDL_Event& e);
    
    virtual void scrollWheel(const SDL_Event& e);
    
    virtual void keyDown(const SDL_Event& e);
    virtual void keyUp(const SDL_Event& e);
};


} //namespace SL



#endif