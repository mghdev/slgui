#ifndef SILHOUETTE_INCLUDE_BUTTON_H_
#define SILHOUETTE_INCLUDE_BUTTON_H_

#include <memory> //shared_ptr
#include <functional> //function

#include "SLTextView.hpp"

namespace SL {

class Button : public View
{
protected:
    std::shared_ptr<TextView> label_view;
    
public:
    typedef enum State {
        STATE_NONE      = 0x00,
        STATE_HOVERED   = 0x01,
        STATE_PRESSED   = 0x02,
        STATE_SELECTED  = 0x04
    } State;
    int8_t state = STATE_NONE;
    int8_t visual_states = STATE_NONE | STATE_HOVERED;
    
    std::function<void()> button_press_action = [](){};
    
    Button(Vec2F size, SDL_Renderer* renderer);
    
    void setLabel(std::string new_label);
    std::string getLabel();
    
    virtual void enterState(State s);
    virtual void exitState(State s);
    
    virtual void drawContent(SDL_Renderer* renderer) override;
    
    // Button hides events from its subviews
    virtual View* hitTest(Point p) override;
    
    virtual bool acceptsFirstResponder() override {return true;};
    virtual bool becomeFirstResponder() override;
    virtual bool resignFirstResponder() override;
    
    virtual void leftMouseDown(const SDL_Event& event) override;
    virtual void leftMouseUp(const SDL_Event& event) override;
    virtual void leftMouseDragged(const SDL_Event& event) override;
    
    virtual void mouseEntered(const SDL_Event& event);
    virtual void mouseExited(const SDL_Event& event);
    
    virtual void keyDown(const SDL_Event& event) override;
};

} //namespace SL

#endif