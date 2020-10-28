#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

#include "../data_classes/data_classes.hpp"

enum MouseButton { LEFT, MIDDLE, RIGHT, UNDEFINED_BUTTON };

enum EVENT_TYPE {
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_HOVER,
    MOUSE_MOVED,
    WINDOW_CLOSED,
    UNDEFINED_EVENT
};

struct MouseButtonEvent {
    Position pos;
    MouseButton mouse_button;
};

struct MouseHoverEvent {
    Position pos;
};

struct MouseMoveEvent {
    Position pos;
};

struct Event {
    uint32_t type;
    union {
        MouseButtonEvent mouse_button_event;
        MouseHoverEvent mouse_hover_event;
        MouseMoveEvent mouse_move_event;
    };
};

#endif
