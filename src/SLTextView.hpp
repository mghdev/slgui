#ifndef SILHOUETTE_INCLUDE_TEXTVIEW_H_
#define SILHOUETTE_INCLUDE_TEXTVIEW_H_

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

#include "SLView.hpp"

namespace SL {

class TextView : public View
{
protected:
    static constexpr auto min_cursor_width = 2;

    bool allows_editing = true;
    bool currently_editing = false;
    TTF_Font* font = nullptr;
    TTF_TextEngine* rendering_engine = nullptr;
    TTF_Text* internal_text = nullptr;
    int cursor = 0;
    
    void drawCursor(SDL_Renderer* renderer);
    
    void cursorVertical(int rows);
    void cursorUp();
    void cursorDown();
    void cursorLeft();
    void cursorRight();
    void backspace();
    
    void handleKeyPress(const SDL_Event& event);
public:
    TextView(Vec2F size, SDL_Renderer* renderer);
    
    virtual ~TextView() override;
    // delete-ing all of these while the class is WiP
    // when the design is more settled, I will probably implement some of them
    // just very annoying to have to update all the time
    TextView(const TextView&) = delete;
    TextView& operator=(const TextView&) = delete;
    TextView(TextView&& other) = delete;
    // TextView(TextView&& other) noexcept;
    TextView& operator=(TextView&& other) = delete;
    
    virtual void drawContent(SDL_Renderer* renderer) override;
    
    void setAllowsEditing(bool val);
    void moveCursor(int new_pos);
    
    std::string getString();
    void setString(std::string s);
    void deleteSubstring(int pos, int len);
    void insertTextAtCursor(const char* str, int len);
    
    virtual bool acceptsFirstResponder() override;
    virtual bool becomeFirstResponder() override;
    virtual bool resignFirstResponder() override;
    virtual void leftMouseDown(const SDL_Event& event) override;
    virtual void leftMouseUp(const SDL_Event& event) override;
    virtual void leftMouseDragged(const SDL_Event& event) override;
    virtual void keyDown(const SDL_Event& event) override;
    virtual void keyHold(const SDL_Event& event) override;
    virtual void textInput(const SDL_Event& event) override;
    
    virtual void otherMouseDown(const SDL_Event& event) override {};
    virtual void otherMouseUp(const SDL_Event& event) override {};
    virtual void otherMouseDragged(const SDL_Event& event) override {};
    virtual void mouseMoved(const SDL_Event& event) override {};
};

} //namespace SL

#endif