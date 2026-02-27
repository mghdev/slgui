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
    virtual void appDidInitialize(Application* app) {};
    virtual void appWillTerminate(Application* app) {};    
};

class Application : public Responder
{
protected:
public:
    std::unique_ptr<AppDelegate> delegate = nullptr;
    std::unique_ptr<Window> main_window = nullptr;
    
    Application(std::shared_ptr<AppDelegate> delegate);
    ~Application();
    
    int run();
};

} // namespace SL

#endif