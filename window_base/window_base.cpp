#include "window_base.hpp"

/*---------------------------------------*/
/*              Window                   */
/*---------------------------------------*/

Window::Window() = default;

void Window::add_child_window(std::unique_ptr<Window>& child) {
  subwindows.push_back(std::move(child));
}

Window::~Window() {}

void Window::handle_event(Event* event) {}

void Window::delete_child_window(
    std::list<std::unique_ptr<Window>>::iterator child) {
    subwindows.erase(child);
}
