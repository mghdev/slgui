#include "SLApplication.hpp"
#include "SLWindow.hpp"

namespace SL {

Application::Application()
{
    SDL_Init(0);
    main_window = std::make_shared<Window>(960,720);
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


// template <typename ET,typename E>
// void _SLEventSystem<ET,E>::addEventWatch(ET type,_Responder<E>* resp)
// {
//     responders.insert({type,{}});
//     responders[type].push_back(resp);
// }

// template <typename ET,typename E>
// void _SLEventSystem<ET,E>::delEventWatch(ET type,_Responder<E>* resp)
// {
//     if(responders.contains(type)) {
//         auto vec = responders[type];
//         vec.erase(std::find(begin(vec),end(vec),resp));
//     }
// }

// template <typename ET,typename E>
// void _SLEventSystem<ET,E>::handleEvent(const E& event)
// {
//     if(responders.contains(event.type)) {
//         auto& vec = responders[event.type];
//         for(auto& resp : vec) {
//             resp->respondToEvent(event);
//         }
//     }
// }
// template class _SLEventSystem<Uint32,SDL_Event>;