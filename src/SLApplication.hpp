#ifndef SILHOUETTE_INCLUDE_APPLICATION_H_
#define SILHOUETTE_INCLUDE_APPLICATION_H_

#include <unordered_map>
#include <string>
#include <memory>

#include <SDL.h>

#include "SLEvent.hpp"

namespace SL {

class Window;
class Application : public Responder
{
protected:
public:
    // SLConfigModel config;
    // std::unordered_map<std::string,SLConfigType> config;
    std::shared_ptr<Window> main_window;
    
    Application();
    ~Application();
    
    int run();
};

} // namespace SL


// template <typename ET,typename E>
// class _SLEventSystem
// {
// protected:
//     std::unordered_map<ET,std::vector<_Responder<E>*>> responders;
// public:
//     _SLEventSystem() {};
//     ~_SLEventSystem() {};

//     void handleEvent(const E& e);

//     // NOTE: the same watch may be added multiple times
//     // if there have been multiple copies of a watch added, they are deleted one at a time
//     // void addEventWatch(ET type,const SLEventWatchConditional& resp);
//     // void delEventWatch(ET type,const SLEventWatchConditional& resp);
//     void addEventWatch(ET type,_Responder<E>* resp);
//     void delEventWatch(ET type,_Responder<E>* resp);
// };
// typedef _SLEventSystem<Uint32,SDL_Event> SLEventSystem;  //for convenience


#endif