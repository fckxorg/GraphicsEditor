#include "app.hpp"

std::unique_ptr<Window> App::root_window;
bool App::open = true;

void App::run() {
  Event* event = nullptr;

  // TODO timer event generation
  while (open) {
    event = Renderer::poll_event();
    while (event) {
      if (event->get_type() == WINDOW_CLOSED) {
        open = false;
      }

      EventQueue::add_event(event);
      event = Renderer::poll_event();
    }

    while (!EventQueue::empty()) {
      event = EventQueue::get_event();
      root_window->handle_event(event);
    }

    root_window->render();
    Renderer::draw_delayed();
    Renderer::show();
    Renderer::clear();
  }
}

void App::init(Size size, const char* name) {
  Renderer::init(size, name);
  open = true;
}

void App::deinit() { Renderer::deinit(); 
    InstrumentManager::deinit();}

void App::set_root_window(std::unique_ptr<Window>& window) {
  root_window = std::move(window);
  SubscriptionManager::set_system_event_sender(root_window.get());
}
