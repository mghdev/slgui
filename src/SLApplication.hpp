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

class Application;
class AppDelegate
{
public:
    // override to set up initial model and view hierarchy
    virtual void appDidInitialize(Application* app) {};
    
    // override to clean up before app closes
    virtual void appWillTerminate(Application* app) {};    
};

class Application : public Responder
{
protected:
public:
    std::shared_ptr<AppDelegate> delegate;
    std::unique_ptr<Window> main_window = nullptr;
    
    Application(std::shared_ptr<AppDelegate> delegate);
    ~Application();
    
    int run();
};

} // namespace SL

#endif