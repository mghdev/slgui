#include "SLApplication.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include "SLWindow.hpp"

namespace SL {

Application::Application(std::shared_ptr<AppDelegate> new_delegate)
{
    SDL_Init(0);
    TTF_Init();
    delegate = std::move(new_delegate);
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
            default:
                main_window->sendEvent(sdl_event);
                break;
        }
        main_window->displayIfNeeded();
    }
    return 0;
}

} //namespace SL