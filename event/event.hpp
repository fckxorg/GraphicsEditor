#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

#include "../data_classes/data_classes.hpp"

enum SYSTEM_EVENT { MOUSE_BUTTON, MOUSE_MOVE, WINDOW_CLOSED };

class Event {
   private:
    uint32_t type;

   public:
    uint32_t get_type() {
        return type;
    }

    Event(uint32_t type) : type(type) {}
    virtual ~Event() = default;
};

class WindowClosedEvent : public Event {
    public:
        WindowClosedEvent() : Event(WINDOW_CLOSED) {}
};

class MouseButtonEvent : public Event {
   public:
    enum MouseButton { LEFT, MIDDLE, RIGHT, UNDEFINED_BUTTON };
    enum Action { PRESSED, RELEASED };
    Position pos;
    MouseButton button;
    Action action;
    MouseButtonEvent(Position pos, MouseButton button, Action action) : Event(MOUSE_BUTTON), pos(pos), button(button), action(action) {}
};

class MouseMoveEvent : public Event {
   public:
    Position pos;

    MouseMoveEvent(Position pos) : Event(MOUSE_MOVE), pos(pos){}
};

#endif
