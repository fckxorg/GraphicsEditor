#include <queue>
#include <cassert>

#include "../event/event.hpp"

class EventQueue {
 private:
  static std::queue<Event*> event_queue;

  static Event* get_event();
  static void add_event(Event* new_event);
  static bool empty();

 public:
  EventQueue() = delete;

  friend class App;
  friend class Window;
};
