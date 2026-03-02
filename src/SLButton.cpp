#include "SLButton.hpp"

#include "SLGeometry.hpp"

namespace SL {

static constexpr auto DEFAULT_BORDER_COLOR = Color(25,125,150,255);

Button::Button(Vec2F size, SDL_Renderer* renderer) : 
    View(size),
    label_view(std::make_shared<TextView>(size,renderer))
{
    addSubview(label_view);
    setBackgroundColor(COLOR::DARKMODE_BACKGROUND);
    setBorderWidthAndColor(2,DEFAULT_BORDER_COLOR);
}

void Button::setLabel(std::string new_label)
{
    label_view->setString(new_label);
}

std::string Button::getLabel()
{
    return label_view->getString();
}

void Button::enterState(State s)
{
    auto new_state = state | s;
    if(visual_states & s && new_state != state) {
        requestDisplay();
    }
    state = new_state;
}

void Button::exitState(State s)
{
    auto new_state = state & ~s;
    if(visual_states & s && new_state != state) {
        requestDisplay();
    }
    state = new_state;
}

void Button::drawContent(SDL_Renderer* renderer)
{
    
}

View* Button::hitTest(Point p)
{
    return isPointInRect(p,frame) ? this : nullptr;
}

bool Button::becomeFirstResponder()
{
    enterState(STATE_SELECTED);
    return true;
}

bool Button::resignFirstResponder()
{
    exitState(STATE_SELECTED);
    return true;
}

void Button::leftMouseDown(const SDL_Event& event)
{
    enterState(STATE_PRESSED);
}

void Button::leftMouseUp(const SDL_Event& event)
{
    if(state & STATE_PRESSED) {
        action();
    }
    exitState(STATE_PRESSED);
}

void Button::mouseEntered(const SDL_Event& event)
{
    enterState(STATE_HOVERED);
}

void Button::mouseExited(const SDL_Event& event)
{
    exitState(STATE_HOVERED);
}

void Button::leftMouseDragged(const SDL_Event& event)
{
    
}

void Button::keyDown(const SDL_Event& event)
{
    if(state & STATE_SELECTED && event.key.key == SDLK_RETURN) {
        action();
    }
}



} //namespace SL
