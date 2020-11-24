#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <bits/stdint-uintn.h>
#include <stdio.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <list>
#include <memory>
#include <vector>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"
#include "../subscription_manager/subscription_manager.hpp"
#include "../window_base/window_base.hpp"

#ifdef NCURSES_ENGINE
#include "../ncurses_engine/ncurses_engine.hpp"
#endif

#ifdef SFML_ENGINE
#include "../sfml_engine/sfml_engine.hpp"
#endif

extern const uint8_t PRESS_FADE_DELTA;
extern const uint8_t CONTROLS_COLOR_DELTA;
extern const float SCROLLBAR_BUTTON_RATIO;
extern const float LINESPACING_COEFF;
extern const float SCROLLBAR_SIZE_RATIO;

enum DIRECTION { UP, DOWN };

/*!
 * Interface which should be inherited by any clickable window
 * */

class InterfaceClickable {
 public:
  /*!
   * This method decides which event handler to call
   * @param event event data structure
   * */
  void handle_mouse_button_event(Event* event);
  virtual void onMousePress(MouseButtonEvent* event) = 0;
  virtual void onMouseRelease(MouseButtonEvent* event) = 0;
  virtual ~InterfaceClickable();
};

/*!
 * This interface should be inherited by any draggable window
 * */

class InterfaceDraggable : public InterfaceClickable {
 public:
  virtual void onMouseMove(MouseMoveEvent* event) = 0;
};

/*!
 * Root window of window hierarchy. It is just container, which wouldn't render
 * and is used to send system events.
 * */
class RootWindow : public Window {
 public:
  virtual void render();
  virtual void handle_event(Event* event);
};

class RenderWindow : public Window {
 protected:
  Position pos;
  Size size;
  float rotation;

 public:
  RenderWindow();

  virtual ~RenderWindow();

  RenderWindow(const RenderWindow& other) = delete;

  RenderWindow(Size size, Position pos);

  virtual void render();

  void set_pos(Position pos);

  virtual Position get_position() const;

  void set_rotation(const float rotation);

  float get_rotation() const;

  void move_children(float offset_x, float offset_y);
};

class RectWindow : public RenderWindow {
 protected:
  Color color;
  virtual bool is_point_inside(Position point);

 public:
  RectWindow();
  ~RectWindow();
  RectWindow(Size size, Position pos, Color color);

  void set_color(Color color);

  Color get_color();

  virtual void render() override;
};

class TextWindow : public RenderWindow {
 private:
  Text text;

 public:
  TextWindow();
  TextWindow(Text text, Position pos);

  Text get_text() const;
  void set_text(Text text);

  virtual void render() override;
};

class RectButton : public RectWindow, public InterfaceClickable {
 private:
  Color default_color;
  uint32_t value;

 public:
  RectButton();
  ~RectButton();

  virtual void render() override;
  RectButton(Size size, Position pos, Color color, uint32_t value = 0);

  virtual void handle_event(Event* event) override;

  bool check_boundaries(Position click_pos);

  virtual void onMousePress(MouseButtonEvent* event) override;
  virtual void onMouseRelease(MouseButtonEvent* event) override;
};

class Slider : public RectWindow, public InterfaceDraggable {
 private:
  Color default_color;
  Position last_mouse_pos;
  bool pressed;
  uint16_t step;

  bool horizontal;
  uint16_t lower_bound;
  uint16_t upper_bound;

  int16_t Position::*primary_axis;

  void move(int delta);
  float get_relative_pos();

 public:
  Slider();
  ~Slider();
  Slider(Size size, Position pos, Color color, uint16_t lower_bound,
         uint16_t upper_bound, uint16_t step, bool horizontal = false);

  virtual void onMousePress(MouseButtonEvent* event);
  virtual void onMouseRelease(MouseButtonEvent* event);
  virtual void onMouseMove(MouseMoveEvent* event);

  void onButtonUp();
  void onButtonDown();

  virtual void handle_event(Event* event);
};

class Scrollbar : public RectWindow {
 public:
  Scrollbar();
  ~Scrollbar();

  void handle_event(Event* event) override;
  Scrollbar(Size size, Position pos, Color color, uint16_t viewport_size,
            uint16_t scroll_block_size, uint16_t step, bool horizontal = false);
};

class ScrollableWindow : public RectWindow {
 private:
  float offset_x;
  float offset_y;
  Size inner_container_size;

 public:
  ScrollableWindow(Size viewport_size, Size inner_container_size, Position pos,
                   Color bg_color);
  virtual void render() override;
  virtual void handle_event(Event* event) override;
};

class ScrollableText : public RectWindow {
 private:
  Text text;
  uint16_t whole_block_height;
  float offset;

  uint16_t get_nlines();

 public:
  ScrollableText(Size viewport_size, Position pos, Color bg_color, Text text);
  virtual void render() override;

  void handle_event(Event* event) override;
};

class Canvas : public RectWindow, public InterfaceClickable {
 private:
  Image img;

 public:
  Canvas(Size size, Position pos, Color color);

  virtual void handle_event(Event* event) override;
  void load_from_file(const char* filename);
  void save_to_file(const char* filename);
  virtual void render() override;

  void onMousePress(MouseButtonEvent* event) override;
  void onMouseRelease(MouseButtonEvent* event) override;
  void onMouseMove(MouseMoveEvent* event);
};

class InstrumentManager {
 private:
  static bool application_started;
  static Position last_point;

 public:
  static void start_applying(Position pos) {
    application_started = true;
    last_point = pos;
  }

  static void stop_applying() { application_started = false; }

  static void apply(Image& canvas, Position pos) {
    float k = static_cast<float>(pos.y - last_point.y) / (pos.x - last_point.x);
    float b = pos.y - k * pos.x;

    for (int x = std::min(pos.x, last_point.x);
         x <= std::max(pos.x, last_point.x); x += 1) {
      canvas.setPixel(x, k * x + b, Color(0, 0, 0));
    }

    k = static_cast<float>(pos.x - last_point.x) / (pos.y - last_point.y);
    b = pos.x - k * pos.y;

    for (int y = std::min(pos.y, last_point.y);
         y <= std::max(pos.y, last_point.y); y += 1) {
      canvas.setPixel(k * y + b, y, Color(0, 0, 0));
    }

    last_point = pos;
  }
  static bool is_applying() { return application_started; }
};

#endif
