

#include "SLApplication.hpp"

int main(int argc, char** argv)
{
    auto vc = std::make_unique<SL::ViewController>(SL::Rect{0,0,960,720});
    auto vc2 = std::make_unique<SL::ViewController>(SL::Rect{100,100,50,50});
    auto vc3 = std::make_unique<SL::ViewController>(SL::Rect{500,500,42,68});
    vc->addChild(std::move(vc2));
    vc->addChild(std::move(vc3));
    vc->removeChild(1);
    auto app = SL::Application(std::move(vc));
    return app.run();
}
