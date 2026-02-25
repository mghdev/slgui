#ifndef SILHOUETTE_INCLUDE_TEXTVIEW_H_
#define SILHOUETTE_INCLUDE_TEXTVIEW_H_

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

#include "SLView.hpp"

namespace SL {

TTF_Font* defaultFont();

class TextView : public View
{
protected:
    std::string string = "";
    TTF_Font* font = nullptr;
public:
    Color text_color = WHITE;
    
    TextView(Vec2F size, TTF_Font* font = nullptr);
    virtual void drawContent(SDL_Renderer* renderer) override;
    
    void setString(std::string s);
    const std::string& getString();
    
    void setFont(TTF_Font* font);
    TTF_Font* getFont();
};

} //namespace SL

#endif