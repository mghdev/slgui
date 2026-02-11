#include "SLEvent.hpp"

namespace SL {

void Responder::setNextResponder(Responder* r)
{
    next_responder = r;
}

void Responder::respondToEvent(const SDL_Event& e)
{
    if (next_responder) {
        next_responder->respondToEvent(e);
    }
}

} //namespace SL