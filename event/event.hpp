#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

#include "../data_classes/data_classes.hpp"

enum SYSTEM_EVENT {
  MOUSE_BUTTON,
  MOUSE_MOVE,
  WINDOW_CLOSED,
  BUTTON_PRESSED,
  SCROLL,
  SLIDER_MOVE,
  HUE_CHANGED,
  COLOR_CHANGED,
  FADER_MOVE,
  DROPPER_APPLIED,
  KEY_PRESSED,
};

class Event {
 private:
  uint32_t type;

 public:
  uint32_t get_type();
  Event();

  Event(uint32_t type);
  Event(const Event& event);
  virtual ~Event();
};

class WindowClosedEvent : public Event {
 public:
  WindowClosedEvent();
};

class MouseButtonEvent : public Event {
 public:
  enum MouseButton { LEFT, MIDDLE, RIGHT, UNDEFINED_BUTTON };
  enum Action { PRESSED, RELEASED };
  Position pos;
  MouseButton button;
  Action action;
  MouseButtonEvent(Position pos, MouseButton button, Action action);
};

class MouseMoveEvent : public Event {
 public:
  Position pos;

  MouseMoveEvent(Position pos);
};

class ButtonPressEvent : public Event {
 public:
  uint32_t value;
  ButtonPressEvent(uint32_t value);
};

class ScrollEvent : public Event {
 public:
  float position;
  ScrollEvent(float position);
};

class SliderMoveEvent : public Event {
 public:
  float position;
  SliderMoveEvent(float position);
};

class HueChangedEvent : public Event {
 public:
  float hue;
  HueChangedEvent(float hue);
};

class ColorChangedEvent : public Event {
 public:
  Color color;
  ColorChangedEvent(Color color);
};

class FaderMoveEvent : public Event {
 public:
  float pos_x;
  float pos_y;
  FaderMoveEvent(float pos_x, float pos_y);
};

class DropperEvent : public Event {
 public:
  Color color;
  DropperEvent(Color color);
};

class KeyPressedEvent : public Event {
 public:
  char key;
  KeyPressedEvent(char key);
};

#endif
