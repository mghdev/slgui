#include <format> //format

#include "SLApplication.hpp"
#include "SLTextView.hpp"
#include "SLButton.hpp"

class LinesView : public SL::View
{
public:
    using SL::View::View;
    virtual void drawContent(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer,125,125,125,255);
        SDL_RenderLine(renderer,0,view_size.y/2,view_size.x,view_size.y/2);
        SDL_RenderLine(renderer,view_size.x/2,0,view_size.x/2,view_size.y);
    }
};

class Incrementer : public SL::ViewController
{
protected:
    int count = 0;
    std::shared_ptr<SL::TextView> count_display;
public:
    Incrementer(std::shared_ptr<SL::TextView> view) : SL::ViewController(view),count_display(view) {
        view->setString(std::format("{}",count));
    }
    void increment() {
        ++count;
        count_display->setString(std::format("{}",count));
    }
};

class ColorCycler : public SL::ViewController
{
protected:
    static constexpr int num_colors = 5;
    
    int color_idx = 0;
    SL::Color colors[num_colors] = {SL::COLOR::DARKMODE_BACKGROUND,{110,55,55,255},{55,110,55,255},{55,55,110,255},{110,110,110,255}};
    
    void setColorIdx(int i) {
        color_idx = SL::clamp(0,i,num_colors-1);
        view->setBackgroundColor(colors[color_idx]);
    }
    
public:
    ColorCycler(SL::Rect r, int idx = 0) : SL::ViewController(r), color_idx(idx) {
        view->setBackgroundColor(colors[color_idx]);
    }
    ColorCycler(std::shared_ptr<SL::View> view, int idx = 0) : SL::ViewController(view), color_idx(idx) {
        view->setBackgroundColor(colors[color_idx]);
    }
    void nextColor() {
        setColorIdx((color_idx+1)%num_colors);
        view->requestDisplay();
    }
    virtual void leftMouseDown(const SDL_Event& event) override {
        nextColor();
    }
    virtual void keyDown(const SDL_Event& event) override {
        switch(event.key.key) {
            case SDLK_W:
                setColorIdx(4);
                break;
            case SDLK_R:
                setColorIdx(1);
                break;
            case SDLK_G:
                setColorIdx(2);
                break;
            case SDLK_B:
                setColorIdx(3);
                break;
            case SDLK_N:
                setColorIdx(0);
                break;
            case SDLK_SPACE:
                nextColor();
                break;
            default:
                break;
        }
        view->requestDisplay();
    }
};

class DemoAppDelegate : public SL::AppDelegate
{
public:
    void appDidInitialize(SL::Application* app) override {
        // Set up the window and view hierarchy
        
        // Often, the root view in the hierarchy should simply be a container for subviews
        auto root_vc = std::make_unique<SL::ViewController>(SL::Rect{0,0,960,720});
        root_vc->view->setBackgroundColor(SL::Color{110,110,110,255});
        
        // Some squares that change colors
        auto vc2 = std::make_unique<ColorCycler>(SL::Rect{600,100,100,100},1);
        auto vc3 = std::make_unique<ColorCycler>(std::make_shared<LinesView>(SL::Vec2F{40,40}),3);
        vc3->view->setFrame(SL::Rect{30,30,40,40});
        vc2->addChild(std::move(vc3));
        root_vc->addChild(std::move(vc2));
        
        // The root of the view hierarchy must be constructed before the window
        auto window_ptr = std::make_unique<SL::Window>(std::move(root_vc));
        
        // More views can be added after the window is constructed
        auto tv = std::make_shared<SL::TextView>(SL::Vec2F{500,600},window_ptr->renderer);
        tv->setString("Hello there.\nThis is a TextView.\n\nYou can edit this text.\n\nThe colors of the squares to the right will change if you click on them.");
        tv->setBackgroundColor(SL::COLOR::DARKMODE_BACKGROUND);
        auto vc5 = std::make_unique<SL::ViewController>(std::move(tv));
        vc5->view->setFrame(SL::Rect{20,20,500,600});
        window_ptr->content_vc->addChild(std::move(vc5));
        
        // Uncomment this stuff to see what buttons look like so far
        // auto counter = std::make_shared<SL::TextView>(SL::Vec2F{150,40},window_ptr->renderer);
        // auto incr = std::make_unique<Incrementer>(counter);
        // incr->view->setFrame({600,350,150,40});
        
        // auto button = std::make_shared<SL::Button>(SL::Vec2F{150,40},window_ptr->renderer);
        // button->setFrame(SL::Rect{600,400,150,40});
        // button->setLabel("Click Me!");
        
        // auto pointer = incr.get();
        // button->action = [pointer](){
        //     pointer->increment();
        // };
        
        // // Subviews can be added without a view controller
        // window_ptr->content_view->addSubview(std::move(button));
        // window_ptr->content_vc->addChild(std::move(incr));
        
        // Finish by moving the window into app
        app->main_window = std::move(window_ptr);
    }
};

int main(int argc, char** argv)
{
    auto app_delegate = std::make_shared<DemoAppDelegate>();
    auto app = SL::Application(std::move(app_delegate));
    return app.run();
}
