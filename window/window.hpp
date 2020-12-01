#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>
#include <unistd.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <list>
#include <memory>
#include <vector>

#include "../color_utilities/hsvrgb.hpp"
#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"
#include "../instruments_manager/instruments_manager.hpp"
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

  Size get_size();
  void set_size(Size size);

  Color get_color();

  virtual void render() override;
};

class TextWindow : public RenderWindow {
 protected:
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

  int16_t Position::*primary_axis;

  void move(int delta);

 protected:
  void move_relative(float offset);
  float get_relative_pos();

  uint16_t lower_bound;
  uint16_t upper_bound;

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
  enum ACTIONS { SAVE };

  Canvas(Size size, Position pos, Color color);

  virtual void handle_event(Event* event) override;
  void load_from_file(const char* filename);
  void save_to_file(const char* filename);
  virtual void render() override;

  void onMousePress(MouseButtonEvent* event) override;
  void onMouseRelease(MouseButtonEvent* event) override;
  void onMouseMove(MouseMoveEvent* event);

  friend class HUEselector;
  friend class SVselector;
};

class Sprite : public RenderWindow {
 private:
  Texture texture;
  Position pos;

 public:
  Sprite(Texture text, Position pos);
  virtual void render() override;
  virtual void handle_event(Event* event) override;
};

class HUEselector : public RenderWindow {
 private:
  Size size;

 public:
  HUEselector(Size size, Position pos);
  virtual void handle_event(Event* event) override;
};

class SVselector : public RenderWindow {
 private:
  Size size;
  Canvas* canvas;

  void redraw_canvas(float hue);

 public:
  SVselector(Size size, Position pos);
  virtual void handle_event(Event* event) override;
};

class Fader : public RectWindow, public InterfaceDraggable {
 private:
  bool pressed;

 protected:
  Position lower_bound;
  Position upper_bound;

 public:
  Fader(Size size, Position pos, Color color, Position lower_bound,
        Position upper_bound);
  virtual void handle_event(Event* event) override;
  virtual void onMousePress(MouseButtonEvent* event) override;
  virtual void onMouseRelease(MouseButtonEvent* event) override;
  virtual void onMouseMove(MouseMoveEvent* event) override;
  virtual void render() override;
};

class SVFader : public Fader {
 public:
  SVFader(Size size, Position pos, Color color, Position lower_bound,
          Position upper_bound);
  virtual void handle_event(Event* event) override;
};

class HueSlider : public Slider {
 public:
  HueSlider(Size size, Position pos, Color color, uint16_t lower_bound,
            uint16_t upper_bound, uint16_t step, bool horizontal = false);

  void handle_event(Event* event) override;
};

class Inputbox : public RectWindow, public InterfaceClickable {
 private:
  Text input_text;
  int cursor_pos;
  std::string input_value;
  bool active;

 public:
  Inputbox(Size size, Position pos, Color color, uint16_t character_size,
           const char* font_path, Color text_color);
  virtual void handle_event(Event* event) override;
  virtual void render() override;
  virtual void onMousePress(MouseButtonEvent* event) override;
  virtual void onMouseRelease(MouseButtonEvent* event) override;
};

class DialogWindow : public RectWindow {
  Color outline_color;
  int16_t outline_thickness;

 protected:
  Window* creator;

 public:
  DialogWindow(Size size, Position pos, Color color, Color outline_color,
               int16_t outline_thickness, Window* creator);
  void render() override;
  void handle_event(Event* event) override;
  virtual ~DialogWindow();
};

class DialogSaveWindow : public DialogWindow {
 public:
  DialogSaveWindow(Size size, Position pos, Color color, Color outline_color,
                   int16_t outline_thickness, Window* creator);
};

class SaveButton : public RectButton {
 public:
  SaveButton(Size size, Position pos, Color color);
  virtual void onMouseRelease(MouseButtonEvent* event) override;
};

#endif
