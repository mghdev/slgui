#ifndef SILHOUETTE_INCLUDE_DRAWING_H_
#define SILHOUETTE_INCLUDE_DRAWING_H_

#include <SDL3/SDL.h>

namespace SL {

void SDL_RenderThickRect(SDL_Renderer* renderer, SDL_FRect r,float thickness)
{
    auto rect_left = SDL_FRect{r.x,r.y,thickness,r.h};
    auto rect_right = SDL_FRect{r.x+r.w-thickness,r.y,thickness,r.h};
    auto rect_top = SDL_FRect{r.x,r.y,r.w,thickness};
    auto rect_bottom = SDL_FRect{r.x,r.y+r.h-thickness,r.w,thickness};
    
    SDL_RenderFillRect(renderer,&rect_left);
    SDL_RenderFillRect(renderer,&rect_right);
    SDL_RenderFillRect(renderer,&rect_top);
    SDL_RenderFillRect(renderer,&rect_bottom);
}

} //namespace SL

#endif