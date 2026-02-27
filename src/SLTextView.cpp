#include "SLTextView.hpp"

#include <format> //format

namespace SL {

extern "C" const char Roboto_Regular_ttf[];
extern "C" const size_t Roboto_Regular_ttf_size;
static TTF_Font* _DEFAULT_FONT = nullptr;
TTF_Font* sharedDefaultFont()
{
    if(!_DEFAULT_FONT) {
        auto fontio = SDL_IOFromMem((void*)Roboto_Regular_ttf,Roboto_Regular_ttf_size);
        _DEFAULT_FONT = TTF_OpenFontIO(fontio,true,20);
        TTF_SetFontWrapAlignment(_DEFAULT_FONT,TTF_HORIZONTAL_ALIGN_CENTER);
    }
    return _DEFAULT_FONT;
}

TextView::TextView(Vec2F size, SDL_Renderer* renderer) : 
    View(size), 
    font(sharedDefaultFont()), 
    rendering_engine(TTF_CreateRendererTextEngine(renderer)),
    rendered_text(TTF_CreateText(rendering_engine,font,"",0))
{
    
}

void TextView::drawContent(SDL_Renderer* renderer)
{
    TTF_DrawRendererText(rendered_text,0,0);
}

void TextView::setString(std::string s)
{
    TTF_SetTextString(rendered_text,s.c_str(),s.length());
    requestDisplay();
}

const std::string& TextView::getString()
{
    return std::string(rendered_text->text);
}

void TextView::setFont(TTF_Font* new_font)
{
    font = new_font;
}

TTF_Font* TextView::getFont()
{
    if(!font) {
        font = sharedDefaultFont();
    }
    return font;
}

void TextView::keyDown(const SDL_Event& e)
{
    
}

void TextView::keyUp(const SDL_Event& e)
{
    
}


} //namespace SL