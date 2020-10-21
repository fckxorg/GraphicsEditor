#include "app.hpp"

template<typename RenderEngine> 
App<RenderEngine>::App() = default;

template<typename RenderEngine>
App<RenderEngine>::~App() = default;

template<typename RenderEngine>
void App<RenderEngine>::event_loop() {
   // render the window system
   RenderEngine(root_window.get());
}
