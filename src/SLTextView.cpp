#include "SLTextView.hpp"

#include <utility> //exchange
#include <cstring> //strlen
#include "SLWindow.hpp"

namespace SL {

extern "C" const char SpaceMono_Regular_ttf[];
extern "C" const size_t SpaceMono_Regular_ttf_size;
static TTF_Font* _DEFAULT_FONT = nullptr;
TTF_Font* sharedDefaultFont()
{
    if(!_DEFAULT_FONT) {
        auto fontio = SDL_IOFromConstMem((void*)SpaceMono_Regular_ttf,SpaceMono_Regular_ttf_size);
        _DEFAULT_FONT = TTF_OpenFontIO(fontio,true,20);
    }
    return _DEFAULT_FONT;
}

TextView::TextView(Vec2F size, SDL_Renderer* renderer) : 
    View(size), 
    font(sharedDefaultFont()), 
    rendering_engine(TTF_CreateRendererTextEngine(renderer)),
    internal_text(TTF_CreateText(rendering_engine,font,"",0))
{
    TTF_SetTextWrapWidth(internal_text,size.x);
    TTF_SetTextWrapWhitespaceVisible(internal_text,true);
}

TextView::~TextView()
{
    // Font is not owned by the TextView
    TTF_DestroyText(internal_text);
    TTF_DestroyRendererTextEngine(rendering_engine);
}

// TODO: bring these back when design is more stable
// TextView::TextView(TextView&& other) noexcept :
//     View(std::move(other)),
//     rendering_engine(   std::exchange(other.rendering_engine,nullptr)),
//     internal_text(      std::exchange(other.internal_text,nullptr)),
//     allows_editing(     std::move(other.allows_editing)),
//     currently_editing(  std::move(other.currently_editing)),
//     font(               std::move(other.font)),
//     cursor(             std::move(other.cursor))
// {
    
// }

void TextView::drawCursor(SDL_Renderer* renderer)
{
    TTF_SubString substr;
    TTF_GetTextSubString(internal_text,cursor,&substr);
    
    SDL_BlendMode saved;
    SDL_GetRenderDrawBlendMode(renderer,&saved);
    
    auto rect = SDL_FRect{
        static_cast<float>(substr.rect.x),
        static_cast<float>(substr.rect.y),
        static_cast<float>(substr.rect.w == 0 ? min_cursor_width : substr.rect.w),
        static_cast<float>(substr.rect.h)};
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,200,100,100,120);
    SDL_RenderFillRect(renderer,&rect);
    
    SDL_SetRenderDrawBlendMode(renderer,saved);
}

void TextView::drawContent(SDL_Renderer* renderer)
{
    TTF_DrawRendererText(internal_text,0,0);
    if(currently_editing) {
        drawCursor(renderer);
    }
}

void TextView::setAllowsEditing(bool val)
{
    allows_editing = val;
    currently_editing = allows_editing && currently_editing;
}

void TextView::moveCursor(int new_pos)
{
    auto old = cursor;
    if(!internal_text->text) {
        cursor = 0;
    }
    else {
        cursor = clamp(0,new_pos,static_cast<int>(std::strlen(internal_text->text)));
    }
    if(old != cursor) {
        requestDisplay();
    }
}

std::string TextView::getString()
{
    return std::string(internal_text->text);
}

void TextView::setString(std::string s)
{
    TTF_SetTextString(internal_text,s.c_str(),s.length());
    requestDisplay();
}

void TextView::deleteSubstring(int pos, int len)
{
    TTF_DeleteTextString(internal_text,pos,len);
    if(cursor > pos && cursor <= pos+len) {
        moveCursor(pos);
    }
    else {
        requestDisplay();
    }
}

void TextView::insertTextAtCursor(const char* str, int len)
{
    TTF_InsertTextString(internal_text,cursor,str,len);
    cursor += SDL_utf8strlen(str);
    requestDisplay();
}

void TextView::cursorVertical(int rows)
{
    if(!internal_text->text) {
        cursor = 0;
    }
    TTF_SubString substr,current_line,other_line;
    TTF_GetTextSubString(internal_text,cursor,&substr);
    TTF_GetTextSubStringForLine(internal_text,substr.line_index+rows,&other_line);
    TTF_GetTextSubStringForLine(internal_text,substr.line_index,&current_line);
    
    
    auto current_line_glyph_offset = SDL_utf8strnlen(internal_text->text+current_line.offset,substr.offset-current_line.offset);
    
    auto glyphs = 0;
    size_t bytes = other_line.length-1;
    const char* other_line_str = internal_text->text+other_line.offset;
    while(glyphs < current_line_glyph_offset && SDL_StepUTF8(&other_line_str,&bytes)) {
        ++glyphs;
    }
    
    moveCursor(other_line_str-internal_text->text);
}

void TextView::cursorUp()
{
    cursorVertical(-1);
}

void TextView::cursorDown()
{
    cursorVertical(+1);
}

void TextView::cursorLeft()
{
    TTF_SubString substr;
    TTF_GetTextSubString(internal_text,cursor-1,&substr);
    moveCursor(cursor-substr.length);  //substr.length is 0 if we are at beginning
}

void TextView::cursorRight()
{
    TTF_SubString substr;
    TTF_GetTextSubString(internal_text,cursor+1,&substr);
    if(substr.flags & TTF_SUBSTRING_TEXT_END) {
        moveCursor(substr.offset);
    }
    else {
        moveCursor(cursor+substr.length);
    }
}

void TextView::backspace()
{
    TTF_SubString substr;
    TTF_GetTextSubString(internal_text,cursor-1,&substr);
    deleteSubstring(substr.offset,substr.length);
}

bool TextView::acceptsFirstResponder()
{
    return true;
}

bool TextView::becomeFirstResponder()
{
    currently_editing = allows_editing;
    if(currently_editing) {
        window->startTextInput();
        requestDisplay();
    }
    return true;
}

bool TextView::resignFirstResponder()
{
    if(currently_editing) {
        currently_editing = false;
        window->stopTextInput();
        requestDisplay();
    }
    return true;
}

void TextView::leftMouseDown(const SDL_Event& event)
{
    if(!currently_editing) {
        return;
    }
    TTF_SubString substr;
    auto rect = transformRectFrom(Rect{event.button.x,event.button.y,0,0},nullptr);
    TTF_GetTextSubStringForPoint(internal_text,rect.x,rect.y,&substr);
    moveCursor(substr.offset);
}

void TextView::leftMouseUp(const SDL_Event& event)
{
    
}

void TextView::leftMouseDragged(const SDL_Event& event)
{
    
}

void TextView::handleKeyPress(const SDL_Event& event)
{
    if(!currently_editing) {
        return;
    }
    switch (event.key.key)
    {
        case SDLK_UP:
            cursorUp();
            break;
        case SDLK_DOWN:
            cursorDown();
            break;
        case SDLK_LEFT:
            cursorLeft();
            break;
        case SDLK_RIGHT: 
            cursorRight();
            break;
        case SDLK_BACKSPACE:
        case SDLK_DELETE:
            backspace();
            break;
        case SDLK_RETURN:
            insertTextAtCursor("\n",0);
            break;
        case SDLK_V:
            if(event.key.mod & SDL_KMOD_CTRL) {
                insertTextAtCursor(SDL_GetClipboardText(),0);
            }
            break;
        default:
            break;
    }
}

void TextView::keyDown(const SDL_Event& event)
{
    handleKeyPress(event);
}

void TextView::keyHold(const SDL_Event& event)
{
    handleKeyPress(event);
}

void TextView::textInput(const SDL_Event& event)
{
    if(!currently_editing) {
        return;
    }
    insertTextAtCursor(event.text.text,0);
}




} //namespace SL