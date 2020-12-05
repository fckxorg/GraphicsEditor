#include "event.hpp"

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>

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

ScrollEvent::ScrollEvent(float position) : Event(SCROLL), position(position) {}

SliderMoveEvent::SliderMoveEvent(float position)
    : Event(SLIDER_MOVE), position(position) {}

HueChangedEvent::HueChangedEvent(float hue) : Event(HUE_CHANGED), hue(hue) {}

ColorChangedEvent::ColorChangedEvent(Color color)
    : Event(COLOR_CHANGED), color(color) {}

FaderMoveEvent::FaderMoveEvent(float pos_x, float pos_y)
    : Event(FADER_MOVE), pos_x(pos_x), pos_y(pos_y) {}

DropperEvent::DropperEvent(Color color)
    : Event(DROPPER_APPLIED), color(color) {}

KeyPressedEvent::KeyPressedEvent(KEY key, bool shift, bool ctrl)
    : Event(KEY_PRESSED), key(key), shift(shift), ctrl(ctrl) {}

FileListRebuildEvent::FileListRebuildEvent(std::string name)
    : Event(FILE_LIST_REBUILD), name(name) {}

ContainerSizeChangedEvent::ContainerSizeChangedEvent(int16_t block_size)
    : Event(CONTAINER_SIZE_CHANGED), block_size(block_size) {}

ChangeInputboxValueEvent::ChangeInputboxValueEvent(std::string value)
    : Event(CHANGE_INPUTBOX_VALUE), value(value) {}

CanvasFileEvent::CanvasFileEvent(std::string filename, CanvasAction type)
    : Event(CANVAS_ACTION), filename(filename), type(type) {}

FileChoiceEvent::FileChoiceEvent(std::string filename)
    : filename(filename), Event(FILE_CHOOSEN) {}
