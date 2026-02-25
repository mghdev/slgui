#include "SLTextView.hpp"

#include <format> //format

namespace SL {

extern "C" const char Roboto_Regular_ttf[];
extern "C" const size_t Roboto_Regular_ttf_size;
static TTF_Font* _DEFAULT_FONT = nullptr;
TTF_Font* defaultFont()
{
    if(!_DEFAULT_FONT) {
        auto fontio = SDL_IOFromMem((void*)Roboto_Regular_ttf,Roboto_Regular_ttf_size);
        if(!(_DEFAULT_FONT = TTF_OpenFontIO(fontio,true,24))) {
            auto e = SDL_GetError();
            throw std::runtime_error(std::format("Failed to create backing window: {}",e));
        }
        
    }
    return _DEFAULT_FONT;
}

TextView::TextView(Vec2F size, TTF_Font* new_font) : View(size), font(new_font)
{
}

void TextView::drawContent(SDL_Renderer* renderer)
{
    auto text_surface = TTF_RenderText_Solid_Wrapped(getFont(),string.c_str(),0,SDL_Color{text_color.r,text_color.g,text_color.b,text_color.a},view_size.x);
    auto rendered_text = SDL_CreateTextureFromSurface(renderer,text_surface);
    auto rect = SDL_FRect(0,0,text_surface->w,text_surface->h);
    SDL_RenderTexture(renderer,rendered_text,&rect,&rect);
}

void TextView::setString(std::string s)
{
    string = std::move(s);
    requestDisplay();
}

const std::string& TextView::getString()
{
    return string;
}

void TextView::setFont(TTF_Font* new_font)
{
    font = new_font;
}

TTF_Font* TextView::getFont()
{
    if(!font) {
        font = defaultFont();
    }
    return font;
}


} //namespace SL