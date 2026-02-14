#include "SLEvent.hpp"

namespace SL {

bool Responder::acceptsFirstResponder()
{
    return false;
}

bool Responder::becomeFirstResponder()
{
    return true;
}

bool Responder::resignFirstResponder()
{
    return true;
}

void Responder::respondToEvent(const SDL_Event& e)
{
    if (next_responder) {
        next_responder->respondToEvent(e);
    }
}

void Responder::leftMouseDown(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->leftMouseDown(e);
    }
}

void Responder::rightMouseDown(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->rightMouseDown(e);
    }
}

void Responder::otherMouseDown(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->otherMouseDown(e);
    }
}


void Responder::leftMouseUp(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->leftMouseUp(e);
    }
}

void Responder::rightMouseUp(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->rightMouseUp(e);
    }
}

void Responder::otherMouseUp(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->otherMouseUp(e);
    }
}

void Responder::mouseMoved(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->mouseMoved(e);
    }
}

void Responder::leftMouseDragged(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->leftMouseDragged(e);
    }
}

void Responder::rightMouseDragged(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->rightMouseDragged(e);
    }
}

void Responder::otherMouseDragged(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->otherMouseDragged(e);
    }
}


void Responder::scrollWheel(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->scrollWheel(e);
    }
}


void Responder::keyDown(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->keyDown(e);
    }
}

void Responder::keyUp(const SDL_Event& e)
{
    if(next_responder) {
        next_responder->keyUp(e);
    }
}


} //namespace SL