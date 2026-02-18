#ifndef SILHOUETTE_INCLUDE_APPLICATION_H_
#define SILHOUETTE_INCLUDE_APPLICATION_H_

#include <unordered_map>
#include <string>
#include <memory>

#include <SDL3/SDL.h>

#include "SLEvent.hpp"
#include "SLWindow.hpp"
#include "SLViewController.hpp"

namespace SL {

class Window;
class Application : public Responder
{
protected:
public:
    std::unique_ptr<Window> main_window;
    
    Application(std::unique_ptr<ViewController> vc);
    ~Application();
    
    int run();
};

} // namespace SL

#endif