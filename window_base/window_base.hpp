#ifndef WINDOW_BASE_HPP
#define WINDOW_BASE_HPP

#include <cstdint>
#include <list>
#include <memory>

#include "../event/event.hpp"

class Window {
 private:
  Viewport view;

 public:
  std::list<std::unique_ptr<Window>> subwindows;

  Window();
  virtual ~Window();

  Viewport get_viewport();
  void set_viewport(Viewport view);

  void add_child_window(std::unique_ptr<Window>& child);
  virtual void handle_event(Event* event);
  virtual void render() = 0;
};

#endif
