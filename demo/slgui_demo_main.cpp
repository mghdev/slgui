
#include "SLApplication.hpp"


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
    ColorCycler(SL::Rect r) : SL::ViewController(r) {
        view->background_color = colors[color_idx];
    }
    void nextColor() {
        setColorIdx((color_idx+1)%num_colors);
    }
    virtual void leftMouseDown(const SDL_Event& event) override {
        nextColor();
    }
    virtual void keyDown(const SDL_Event& event) override {
        switch(event.key.keysym.sym) {
            case SDLK_w:
                setColorIdx(0);
                break;
            case SDLK_r:
                setColorIdx(1);
                break;
            case SDLK_g:
                setColorIdx(2);
                break;
            case SDLK_b:
                setColorIdx(3);
                break;
            case SDLK_n:
                setColorIdx(4);
                break;
            case SDLK_SPACE:
                nextColor();
                break;
            default:
                break;
        }
    }
};


int main(int argc, char** argv)
{
    auto vc = std::make_unique<ColorCycler>(SL::Rect{0,0,960,720});
    auto vc2 = std::make_unique<ColorCycler>(SL::Rect{100,100,50,50});
    auto vc3 = std::make_unique<ColorCycler>(SL::Rect{500,500,42,68});
    auto vc4 = std::make_unique<ColorCycler>(SL::Rect(20,20,40,60));
    
    vc2->nextColor();
    vc3->nextColor();
    vc3->nextColor();
    vc4->nextColor();
    vc4->nextColor();
    vc4->nextColor();
    
    vc2->addChild(std::move(vc4));
    vc->addChild(std::move(vc2));
    vc->addChild(std::move(vc3));
    
    auto app = SL::Application(std::move(vc));
    return app.run();
}
