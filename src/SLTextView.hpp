#ifndef SILHOUETTE_INCLUDE_TEXTVIEW_H_
#define SILHOUETTE_INCLUDE_TEXTVIEW_H_

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

#include "SLView.hpp"

namespace SL {

class TextView;
class TextViewDelegate
{
    virtual void textDidChange(TextView* view) = 0;
};

class TextView : public View
{
protected:
    TTF_Font* font = nullptr;
    TTF_TextEngine* rendering_engine = nullptr;
    TTF_Text* rendered_text = nullptr;
public:
    Color text_color = WHITE;
    
    TextView(Vec2F size, SDL_Renderer* renderer);
    virtual void drawContent(SDL_Renderer* renderer) override;
    
    void setString(std::string s);
    const std::string& getString();
    
    void setFont(TTF_Font* font);
    TTF_Font* getFont();
    
    virtual void leftMouseDown(const SDL_Event& e) override {};
    virtual void otherMouseDown(const SDL_Event& e) override {};
    virtual void leftMouseUp(const SDL_Event& e) override {};
    virtual void otherMouseUp(const SDL_Event& e) override {};
    virtual void leftMouseDragged(const SDL_Event& e) override {};
    virtual void otherMouseDragged(const SDL_Event& e) override {};
    virtual void mouseMoved(const SDL_Event& e) override {};
    
    virtual void keyDown(const SDL_Event& e) override;
    virtual void keyUp(const SDL_Event& e) override;
};

} //namespace SL

#endif