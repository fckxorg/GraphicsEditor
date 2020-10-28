#include "event_queue.hpp"

std::queue<Event> EventQueue::event_queue;

bool EventQueue::empty() {
    return event_queue.empty();
}

void EventQueue::add_event(Event new_event) {
    event_queue.push(new_event);
}

Event EventQueue::get_event() {
    Event front_event = event_queue.front();
    event_queue.pop();
    return front_event;
}

