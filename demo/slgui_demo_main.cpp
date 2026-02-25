
#include "SLApplication.hpp"
#include "SLTextView.hpp"

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

class ColorCycler : public SL::ViewController
{
protected:
    static constexpr int num_colors = 5;
    
    int color_idx = 0;
    SL::Color colors[num_colors] = {SL::DARKMODE_BACKGROUND,{200,40,40,255},{40,200,40,255},{40,40,200,255},SL::WHITE};
    
    void setColorIdx(int i) {
        color_idx = SL::clamp(0,i,num_colors-1);
        view->background_color = colors[color_idx];
    }
    
public:
    ColorCycler(SL::Rect r, int idx = 0) : SL::ViewController(r), color_idx(idx) {
        view->background_color = colors[color_idx];
    }
    ColorCycler(std::shared_ptr<SL::View> view, int idx = 0) : SL::ViewController(view), color_idx(idx) {
        view->background_color = colors[color_idx];
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


int main(int argc, char** argv)
{
    auto vc = std::make_unique<ColorCycler>(SL::Rect{0,0,960,720});
    auto vc2 = std::make_unique<ColorCycler>(SL::Rect{100,100,100,100},1);
    auto vc3 = std::make_unique<ColorCycler>(SL::Rect{500,500,42,68},2);
    
    auto vc4 = std::make_unique<ColorCycler>(std::make_shared<LinesView>(SL::Vec2F{40,40}),3);
    vc4->view->setFrame(SL::Rect{30,30,40,40});
    
    vc2->addChild(std::move(vc4));
    vc->addChild(std::move(vc2));
    vc->addChild(std::move(vc3));
    
    auto t = std::make_shared<SL::TextView>(SL::Vec2F{400,200});
    t->setString("Hello there.");
    auto vc5 = std::make_unique<SL::ViewController>(std::move(t));
    vc5->view->setFrame(SL::Rect{550,50,400,200});
    
    vc->addChild(std::move(vc5));
    
    auto app = SL::Application(std::move(vc));
    return app.run();
}
