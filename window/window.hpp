#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <stdio.h>
#include <unistd.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <list>
#include <memory>
#include <vector>

#include "../color_utilities/hsvrgb.hpp"
#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"
#include "../event_queue/event_queue.hpp"
#include "../instruments_manager/instruments_manager.hpp"
#include "../layouts/macro.hpp"
#include "../sfml_engine/sfml_engine.hpp"
#include "../subscription_manager/subscription_manager.hpp"
#include "../window_base/window_base.hpp"

extern const uint8_t PRESS_FADE_DELTA;
extern const uint8_t CONTROLS_COLOR_DELTA;
extern const float SCROLLBAR_BUTTON_RATIO;
extern const float LINESPACING_COEFF;
extern const float SCROLLBAR_SIZE_RATIO;

struct SliderParameters {
  uint16_t lower_bound;
  uint16_t upper_bound;
  uint16_t step;
  bool horizontal;

  SliderParameters();
  SliderParameters(uint16_t lower_bound, uint16_t upper_bound, uint16_t step,
                   bool horizontal);
};

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
  virtual void on_mouse_press(MouseButtonEvent* event) = 0;
  virtual void on_mouse_release(MouseButtonEvent* event) = 0;
  virtual ~InterfaceClickable();
};

/*!
 * This interface should be inherited by any draggable window
 * */

class InterfaceDraggable : public InterfaceClickable {
 public:
  virtual void on_mouse_move(MouseMoveEvent* event) = 0;
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

 public:
  RenderWindow();
  RenderWindow(const RenderWindow& other) = delete;
  RenderWindow(Size size, Position pos);
  virtual ~RenderWindow();

  virtual void render();

  void set_pos(Position pos);
  virtual Position get_position() const;

  virtual void set_size(Size new_size);
  Size get_size() const;
};

class RectWindow : public RenderWindow {
 protected:
  Color color;

  virtual bool is_point_inside(Position point);

 public:
  RectWindow();
  RectWindow(Size size, Position pos, Color color);
  virtual ~RectWindow();

  void set_color(Color color);
  Color get_color() const;

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
 protected:
  Color default_color;
  uint32_t value;

 public:
  RectButton();
  RectButton(Size size, Position pos, Color color, uint32_t value = 0);
  virtual ~RectButton();

  virtual void render() override;

  virtual void handle_event(Event* event) override;
  virtual void on_mouse_press(MouseButtonEvent* event) override;
  virtual void on_mouse_release(MouseButtonEvent* event) override;

  void set_default_color(Color color);
  Color get_default_color() const;
};

class Slider : public RectWindow, public InterfaceDraggable {
 private:
  Color default_color;
  Position last_mouse_pos;
  bool pressed;

  int16_t Position::*primary_axis;

  void move(int delta);
  void on_button(uint32_t value);

 protected:
  void move_relative(float offset);
  float get_relative_pos();

  SliderParameters params;

 public:
  virtual ~Slider();
  Slider(Size size, Position pos, Color color, SliderParameters params);

  virtual void on_mouse_press(MouseButtonEvent* event);
  virtual void on_mouse_release(MouseButtonEvent* event);
  virtual void on_mouse_move(MouseMoveEvent* event);
  virtual void handle_event(Event* event);
};

class Scrollbar : public RectWindow {
 private:
  uint16_t viewport_size;
  uint16_t step;
  bool horizontal;

 public:
  Scrollbar(Size size, Position pos, Color color, uint16_t viewport_size,
            uint16_t scroll_block_size, uint16_t step, bool horizontal = false);
  virtual ~Scrollbar();

  void handle_event(Event* event) override;
  void setup_controls(uint16_t viewport_size, uint16_t scroll_block_size,
                      uint16_t step, bool horizontal);
};

class ScrollableWindow : public RectWindow {
 protected:
  float offset_x;
  float offset_y;
  Size inner_container_size;

 public:
  ScrollableWindow(Size viewport_size, Size inner_container_size, Position pos,
                   Color bg_color);
  virtual void render() override;
  virtual void handle_event(Event* event) override;
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

  void on_mouse_press(MouseButtonEvent* event) override;
  void on_mouse_release(MouseButtonEvent* event) override;
  void on_mouse_move(MouseMoveEvent* event);

  friend class HUEselector;
  friend class SVselector;
};

class Sprite : public RenderWindow {
 private:
  Texture texture;

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
  virtual void on_mouse_press(MouseButtonEvent* event) override;
  virtual void on_mouse_release(MouseButtonEvent* event) override;
  virtual void on_mouse_move(MouseMoveEvent* event) override;
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
  HueSlider(Size size, Position pos, Color color, SliderParameters params);

  void handle_event(Event* event) override;
};

class Inputbox : public RectWindow, public InterfaceClickable {
 private:
  Text input_text;
  int cursor_pos;
  bool active;

 public:
  std::string input_value;

  Inputbox(Size size, Position pos, Color color, uint16_t character_size,
           const char* font_path, Color text_color);
  virtual void handle_event(Event* event) override;
  virtual void render() override;
  virtual void on_mouse_press(MouseButtonEvent* event) override;
  virtual void on_mouse_release(MouseButtonEvent* event) override;
};

class FileList : public ScrollableWindow {
 private:
  std::filesystem::path cur_path;

  void create_entry(Size size, Position pos, std::string name,
                    const char* icon, int type);

 public:
  FileList(Size viewport_size, Size inner_container_size, Position pos,
           Color bg_color);
  void build_entries_list();
  virtual void handle_event(Event* event) override;
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
  virtual ~DialogWindow();
};

class FileDialogWindow : public DialogWindow {
 public:
  FileDialogWindow(Size size, Position pos, Color color, Color outline_color,
                   int16_t outline_thickness, Window* creator,
                   CanvasFileEvent::CanvasAction action);
};

class FileDialogButton : public RectButton {
 private:
  CanvasFileEvent::CanvasAction action;

 public:
  FileDialogButton(Size size, Position pos, Color color,
                   CanvasFileEvent::CanvasAction action);
  virtual void on_mouse_release(MouseButtonEvent* event) override;
  virtual void handle_event(Event* event) override;
};

class DialogEndButton : public RectButton {
 public:
  DialogEndButton(Size size, Position pos, Color color);
  virtual void on_mouse_release(MouseButtonEvent* event) override;
};

class DirectoryEntry : public RectButton {
 private:
  Text text;
  std::string name;
  const char* icon_path;

 public:
  enum Type { REGFILE, FOLDER };
  const int type;

  DirectoryEntry(Size size, Position pos, Color color, Text text,
                 const std::string& name, const char* icon_path, int type);
  virtual void on_mouse_release(MouseButtonEvent* event) override;
  virtual void render() override;

  friend class FileList;
};

class FileDialogEndButton : public DialogEndButton {
 private:
  Inputbox* inputbox;

 public:
  CanvasFileEvent::CanvasAction action;

  FileDialogEndButton(Size size, Position pos, Color color, Inputbox* inputbox,
                      CanvasFileEvent::CanvasAction action);
  virtual void on_mouse_release(MouseButtonEvent* event) override;
};

class PluginToolbar : public Window {
    private:
        Position pos;
        void create_plugin_buttons();
    public:
        PluginToolbar(Position pos);

        virtual void render() override;
        virtual void handle_event(Event* event) override;
};

#endif
