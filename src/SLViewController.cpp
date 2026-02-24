#include "SLViewController.hpp"

#include "algorithm" //find_if, distance
#include "SLGeometry.hpp"

namespace SL {

ViewController::ViewController(Rect r) : ViewController(std::make_shared<View>(r.size()))
{
    view->setFrame(r);
}

ViewController::ViewController(std::shared_ptr<View> view) : view(view)
{
    view->next_responder = this;
}

ViewController::~ViewController()
{
    
}

void ViewController::setView(std::shared_ptr<View> v)
{
    view = std::move(v);
    view->next_responder = this;
}

void ViewController::addChild(std::unique_ptr<ViewController> vc)
{
    view->addSubview(vc->view);
    vc->view->next_responder = vc.get();
    vc->next_responder = view.get();
    vc->parent = this;
    children.push_back(std::move(vc));
}

void ViewController::removeChild(int index)
{
    children[index]->view->removeFromSuperview();
    children[index]->next_responder = nullptr;
    children.erase(std::begin(children)+index);
}

void ViewController::removeFromParent()
{
    auto iter = std::find_if(
        std::begin(parent->children),
        std::end(parent->children),
        [&](const std::unique_ptr<ViewController>& val) {
            return val.get() == this;
        });
    parent->removeChild(std::distance(std::begin(parent->children),iter));
}

} //namespace SL



