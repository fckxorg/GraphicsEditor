#include "window_base.hpp"

/*---------------------------------------*/
/*              Window                   */
/*---------------------------------------*/

Window::Window() = default;

void Window::add_child_window(std::unique_ptr<Window>& child) {
  subwindows.push_back(std::move(child));
}

Window::~Window() = default;

void Window::handle_event(Event* event) {}

Viewport Window::get_viewport() { return view; }

void Window::set_viewport(Viewport view) { this->view = view; }
