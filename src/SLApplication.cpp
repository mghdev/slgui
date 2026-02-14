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
    SDL_Event sdl_event;
    bool running = true;
    while (running && SDL_WaitEvent(&sdl_event))
    {
        switch (sdl_event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEWHEEL:
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