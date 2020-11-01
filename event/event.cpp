#include "event.hpp"

uint32_t Event::get_type() { return type; }

Event::Event(uint32_t type) : type(type) {}

Event::~Event() = default;

WindowClosedEvent::WindowClosedEvent() : Event(WINDOW_CLOSED) {}

MouseButtonEvent::MouseButtonEvent(Position pos, MouseButton button,
                                   Action action)
    : Event(MOUSE_BUTTON), pos(pos), button(button), action(action) {}

MouseMoveEvent::MouseMoveEvent(Position pos) : Event(MOUSE_MOVE), pos(pos) {}

ButtonPressEvent::ButtonPressEvent(uint32_t value)
    : Event(BUTTON_PRESSED), value(value) {}

ScrollEvent::ScrollEvent(int delta) : Event(SCROLL), delta(delta) {}
