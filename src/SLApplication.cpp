#include "SLApplication.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "SLWindow.hpp"

namespace SL {

Application::Application(std::shared_ptr<AppDelegate> delegate)
{
    SDL_Init(0);
    TTF_Init();
    delegate = std::move(delegate);
    delegate->appDidInitialize(this);
}

Application::~Application()
{
    delegate->appWillTerminate(this);
	SDL_Quit();
    TTF_Quit();
}

int Application::run()
{
    if(!main_window) {
        return 1;
    }
    
    main_window->displayIfNeeded();
    
    SDL_Event sdl_event;
    bool running = true;
    while (running && SDL_WaitEvent(&sdl_event))
    {
        switch (sdl_event.type)
        {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            case SDL_EVENT_MOUSE_MOTION:
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            case SDL_EVENT_MOUSE_WHEEL:
                main_window->sendEvent(sdl_event);
                break;
            default:
                break;
        }
        main_window->displayIfNeeded();
    }
    return 0;
}

} //namespace SL