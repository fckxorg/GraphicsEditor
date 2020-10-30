#include "app.hpp"
#include <cstdio>

std::unique_ptr<Window> App::root_window;

void App::run() {
    Event* event = nullptr;

    // TODO timer event generation
    while(true) {
        event = Renderer::poll_event();
        while(event) {
            EventQueue::add_event(event);
            event = Renderer::poll_event();
        }
        
        while(!EventQueue::empty()) {
            event = EventQueue::get_event();
            root_window->handle_event(event);
        }

        root_window->render();
        Renderer::show();
        Renderer::clear();
    }
}

void App::init(Size size, const char* name) {
    Renderer::init(size, name);
}

void App::deinit() {
    Renderer::deinit();
}

void App::set_root_window(std::unique_ptr<Window>& window) {
    root_window = std::move(window);
}
