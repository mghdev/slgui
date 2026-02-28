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

void Responder::respondToEvent(const SDL_Event& event)
{
    if (next_responder) {
        next_responder->respondToEvent(event);
    }
}

void Responder::leftMouseDown(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->leftMouseDown(event);
    }
}

void Responder::rightMouseDown(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->rightMouseDown(event);
    }
}

void Responder::otherMouseDown(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->otherMouseDown(event);
    }
}


void Responder::leftMouseUp(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->leftMouseUp(event);
    }
}

void Responder::rightMouseUp(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->rightMouseUp(event);
    }
}

void Responder::otherMouseUp(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->otherMouseUp(event);
    }
}

void Responder::mouseMoved(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->mouseMoved(event);
    }
}

void Responder::leftMouseDragged(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->leftMouseDragged(event);
    }
}

void Responder::rightMouseDragged(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->rightMouseDragged(event);
    }
}

void Responder::otherMouseDragged(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->otherMouseDragged(event);
    }
}


void Responder::scrollWheel(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->scrollWheel(event);
    }
}


void Responder::keyDown(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->keyDown(event);
    }
}

void Responder::keyUp(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->keyUp(event);
    }
}

void Responder::keyHold(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->keyHold(event);
    }
}

void Responder::textInput(const SDL_Event& event)
{
    if(next_responder) {
        next_responder->textInput(event);
    }
}

} //namespace SL