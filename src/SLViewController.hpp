#ifndef SILHOUETTE_INCLUDE_VIEWCONTROLLER_H_
#define SILHOUETTE_INCLUDE_VIEWCONTROLLER_H_

#include <memory>
#include <vector>

#include "SLBase.hpp"
#include "SLEvent.hpp"
#include "SLView.hpp"

namespace SL {

class ViewController : public Responder
{
private:
    
protected:
    Uint8 dragging_mouse_mask = 0;
public:
    std::shared_ptr<View> view;
    std::vector<std::unique_ptr<ViewController>> children; // ViewControllers are owned by their parent vc (or the window in case of root)
    ViewController* parent = nullptr;

    ViewController(Rect r);
    ViewController(std::shared_ptr<View> view);
    ~ViewController();
    
    void setView(std::shared_ptr<View> v);
    void addChild(std::unique_ptr<ViewController> vc);
    void removeChild(int index);
    void removeFromParent();
};

} //namespace SL



#endif