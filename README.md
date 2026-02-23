# slgui
## MVC, Retained-Mode GUI in C++ 

### About

slgui is meant as a quick way to put together single-window GUI in small, non-realtime C++ projects like custom editors. It implements an MVC view hierarchy that distributes keyboard and mouse events to views or their controllers.

### Adding slgui to a project

> [!NOTE]
> Requires C++20 or later.

 slgui depends on SDL3 and includes it as a submodule to simplify linking on multiple platforms at the cost of increasing build times whenever SDL gets rebuilt. Remember to initialize the submodules in `slgui/external/`

slgui supports building with CMake. To add it to a project, copy the contents of this repository to some subfolder, then in a CMakeLists.txt file:
```
add_subdirectory(path/to/slgui)

target_include_directories(your_target PRIVATE ${SLGUI_INCLUDE_DIRS})

target_link_libraries(your_target slgui)
```

### Using slgui

Using slgui should be familiar to anyone who has used a Model-View-Controller GUI library. See `demo/slgui_demo_main.cpp` for a short example.
First, subclass SL::ViewController:

```
#include "SLViewController.hpp"
class DerivedVC : public SL::ViewController
{
public:
	DerivedVC(SL::Rect r) : SL::ViewController(r) {
		// initial view setup could go here
	}
	
	/*
	Model and other data members probably go here
	Override event handling methods from SL::Responder to respond to keyboard and mouse events.
	*/
}
```

Then, in your program's main function:

```
int main(int argc, char** argv)
{
	// The app's window will inherit the initial size of its root view, 960x720 here
	auto root_vc = std::make_unique<DerivedVC>(SL::Rect{0,0,960,720});
	
	// Do any additional setup of your app's model and view hierarchy, then:
	
	auto app = SL::Application(std::move(root_vc));
	return app.run();
}
```

That's the basic setup! 

slgui projects are expected to use SDL to render their content. To draw things, set your custom view controller's `view` to a subclass of SL::View and override SL::View::drawContent(...)

```
#include "SLView.hpp"
class DerivedView : public SL::View
{
protected:
	SDL_Texture* texture;
public:
	virtual void drawContent(SDL_Renderer* renderer, const Rect& visible_bounds, const Rect& window_coords) override {
		/* 
		Use SDL_Render calls to draw whatever you want
		Most likely, have the view's appearance stored in a SDL_Texture and simply...
		SDL_RenderCopy(renderer,texture,...);
		... from visible_bounds to window_coords
		*/
	}
	
	/*
	Add whatever methods you need to update texture
	*/
}
```

Other implementations are possible, just be careful not to draw outside the window_coords Rect.