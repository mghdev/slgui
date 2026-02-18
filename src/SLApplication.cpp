#include "SLApplication.hpp"
#include "SLWindow.hpp"

namespace SL {

Application::Application(std::unique_ptr<ViewController> vc)
{
    SDL_Init(0);
    main_window = std::make_unique<Window>(std::move(vc));
}

Application::~Application()
{
	SDL_Quit();
}

int Application::run()
{
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