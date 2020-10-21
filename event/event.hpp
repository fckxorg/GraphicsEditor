#ifndef EVENT_HPP
#define EVENT_HPP
#include <cstdint>

class Event {
    public:
        uint32_t type;
};

class MouseClick {
    public:
       Position pos; 
       Button mouse_button;
}

#endif
