

#include "SLApplication.hpp"

int main(int argc, char** argv)
{
    auto view = std::make_shared<SL::View>(SL::Rect{0,0,960,720});
    auto vc = std::make_unique<SL::ViewController>(std::move(view));
    auto app = SL::Application(std::move(vc));
    return app.run();
}
