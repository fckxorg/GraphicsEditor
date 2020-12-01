#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include <queue>
#include <cassert>

#include "../event/event.hpp"

class EventQueue {
 private:
  static std::queue<Event*> event_queue;

  static Event* get_event();
 public:
  static void add_event(Event* new_event);
  static bool empty();

  EventQueue() = delete;

  friend class App;
  friend class Window;
};

#endif
