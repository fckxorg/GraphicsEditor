#include "window_base.hpp"

/*---------------------------------------*/
/*              Window                   */
/*---------------------------------------*/

Window::Window() = default;

void Window::set_event_mask(uint32_t mask) { event_mask = mask; }

void Window::add_child_window(std::unique_ptr<Window>& child) {
    subwindows.push_back(std::move(child));
}

Window::~Window() = default;

void Window::handle_event(Event* event) {
    for (auto& subwindow : subwindows) {
        subwindow->handle_event(event);
    }
}


