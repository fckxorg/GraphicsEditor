#include "window.hpp"

#include <bits/stdint-uintn.h>

#include <cstdio>
#include <list>

const uint8_t PRESS_FADE_DELTA = 50;
const uint8_t CONTROLS_COLOR_DELTA = 30;
const float SCROLLBAR_BUTTON_RATIO = 0.1;
const float LINESPACING_COEFF = 0.08;
const float SCROLLBAR_SIZE_RATIO = 0.07;

/*---------------------------------------*/
/*         Interface Clickable           */
/*---------------------------------------*/

void InterfaceClickable::handle_mouse_button_event(Event* event) {
  assert(event != nullptr);

  auto mouse_button_event = dynamic_cast<MouseButtonEvent*>(event);

  if (mouse_button_event->action == MouseButtonEvent::Action::PRESSED) {
    onMousePress(mouse_button_event);
  }

  if (mouse_button_event->action == MouseButtonEvent::Action::RELEASED) {
    onMouseRelease(mouse_button_event);
  }
}

InterfaceClickable::~InterfaceClickable() = default;

/*---------------------------------------*/
/*              RootWindow               */
/*---------------------------------------*/
void RootWindow::render() {
  for (auto& subwindow : subwindows) {
    subwindow->render();
  }
}
void RootWindow::handle_event(Event* event) { SEND(this, event); };

/*---------------------------------------*/
/*            RenderWindow               */
/*---------------------------------------*/

RenderWindow::RenderWindow() = default;
RenderWindow::~RenderWindow() = default;
RenderWindow::RenderWindow(Size size, Position pos) : size(size), pos(pos) {}

void RenderWindow::set_pos(Position pos) { this->pos = pos; }
Position RenderWindow::get_position() const { return pos; }

void RenderWindow::set_rotation(const float rotation) {
  this->rotation = rotation;
}
float RenderWindow::get_rotation() const { return rotation; }

void RenderWindow::render() {
  for (auto& subwindow : subwindows) {
    subwindow->render();
  }
}

void RenderWindow::move_children(float offset_x, float offset_y) {
  pos.x += offset_x;
  pos.y += offset_y;

  printf("New pos is %d %d", pos.x, pos.y);

  for (auto& subwindow : subwindows) {
    dynamic_cast<RenderWindow*>(subwindow.get())
        ->move_children(offset_x, offset_y);
  }
}

/*---------------------------------------*/
/*              RectWindow               */
/*---------------------------------------*/

RectWindow::RectWindow() = default;
RectWindow::~RectWindow() = default;
RectWindow::RectWindow(Size size, Position pos, Color color)
    : RenderWindow(size, pos), color(color) {}

void RectWindow::render() {
  Renderer::draw_rectangle(size, pos, color);
  RenderWindow::render();
}

void RectWindow::set_color(Color color) { this->color = color; }

Color RectWindow::get_color() { return color; }

bool RectWindow::is_point_inside(Position point) {
  if (point.x < pos.x || point.x > pos.x + size.width) return false;
  if (point.y < pos.y || point.y > pos.y + size.height) return false;

  return true;
}

/*---------------------------------------*/
/*              RectButton               */
/*---------------------------------------*/

RectButton::RectButton() = default;
RectButton::~RectButton() = default;

RectButton::RectButton(Size size, Position pos, Color color, uint32_t value)
    : RectWindow(size, pos, color), default_color(color), value(value) {}

void RectButton::render() { RectWindow::render(); }

void RectButton::onMousePress(MouseButtonEvent* event) {
  assert(event != nullptr);
  if (!is_point_inside(event->pos)) return;

  this->color = RectWindow::get_color() - PRESS_FADE_DELTA;

  SEND(this, new ButtonPressEvent(value));
}

void RectButton::onMouseRelease(MouseButtonEvent* event) {
  assert(event != nullptr);
  this->color = default_color;
}

void RectButton::handle_event(Event* event) {
  if (event->get_type() == SYSTEM_EVENT::MOUSE_BUTTON) {
    handle_mouse_button_event(event);
  }
}

/*---------------------------------------*/
/*              TextWindow               */
/*---------------------------------------*/

TextWindow::TextWindow() = default;
TextWindow::TextWindow(Text text, Position pos) : text(text) { set_pos(pos); }

Text TextWindow::get_text() const { return text; }

void TextWindow::set_text(Text text) { this->text = text; }

void TextWindow::render() {
  Renderer::draw_text(text, pos);
  RenderWindow::render();
}

/*---------------------------------------*/
/*                Slider                 */
/*---------------------------------------*/

Slider::Slider() = default;
Slider::~Slider() = default;

/*!
 * This constructor sets movement axis, so other slider methods will be able to
 * use it later as a pointer to member of Position class
 * @param size size of slider
 * @param pos position of slider
 * @param color slider color
 * @param lower_bound left/top boundary for the slider
 * @param upper_bound right/bottom boundary for the slider
 * @param step step for slider movement (only used to move after button clicks)
 * @param horizontal flag, defining direction of slider movement
 */

Slider::Slider(Size size, Position pos, Color color, uint16_t lower_bound,
               uint16_t upper_bound, uint16_t step, bool horizontal)
    : RectWindow(size, pos, color),
      default_color(color),
      pressed(false),
      lower_bound(lower_bound),
      horizontal(horizontal),
      last_mouse_pos(pos),
      step(step) {
  if (horizontal) {
    primary_axis = &Position::x;
    this->upper_bound = std::max(upper_bound - size.width, 0);
  } else {
    primary_axis = &Position::y;
    this->upper_bound = std::max(upper_bound - size.height, 0);
  }
}

void Slider::move(int delta) {
  uint16_t new_pos =
      std::max(static_cast<uint16_t>(pos.*primary_axis + delta), lower_bound);
  new_pos = std::min(static_cast<uint16_t>(new_pos), upper_bound);

  pos.*primary_axis = new_pos;
}

float Slider::get_relative_pos() {
  return static_cast<float>(pos.*primary_axis - lower_bound) /
         static_cast<float>(upper_bound - lower_bound);
}

void Slider::handle_event(Event* event) {
  assert(event != nullptr);
  switch (event->get_type()) {
    case MOUSE_BUTTON: {
      handle_mouse_button_event(event);
      break;
    }

    case MOUSE_MOVE: {
      auto mouse_move_event = dynamic_cast<MouseMoveEvent*>(event);
      onMouseMove(mouse_move_event);
      break;
    }

    case BUTTON_PRESSED: {
      auto button_press_event = dynamic_cast<ButtonPressEvent*>(event);

      if (button_press_event->value == UP) {
        onButtonUp();
      } else if (button_press_event->value == DOWN) {
        onButtonDown();
      }
      break;
    }
  }
}

void Slider::onButtonUp() {
  move(-step);
  last_mouse_pos = pos;
  SEND(this, new SliderMoveEvent(get_relative_pos()));
}

void Slider::onButtonDown() {
  move(step);
  last_mouse_pos = pos;
  SEND(this, new SliderMoveEvent(get_relative_pos()));
}

void Slider::onMousePress(MouseButtonEvent* event) {
  assert(event != nullptr);
  if (!is_point_inside(event->pos)) return;

  this->color = RectWindow::get_color() - PRESS_FADE_DELTA;
  pressed = true;
  last_mouse_pos = event->pos;
}

void Slider::onMouseMove(MouseMoveEvent* event) {
  assert(event != nullptr);
  if (!pressed) return;

  Position mouse_position = event->pos;
  Position old_pos = pos;

  int mouse_delta = mouse_position.*primary_axis - last_mouse_pos.*primary_axis;
  move(mouse_delta);

  SEND(this, new SliderMoveEvent(get_relative_pos()));

  last_mouse_pos = mouse_position;
}

void Slider::onMouseRelease(MouseButtonEvent* event) {
  assert(event != nullptr);

  this->color = default_color;
  pressed = false;
}

/*---------------------------------------*/
/*             Scrollbar                 */
/*---------------------------------------*/

Scrollbar::Scrollbar() = default;
Scrollbar::~Scrollbar() = default;

void Scrollbar::handle_event(Event* event) {
  assert(event != nullptr);

  if (event->get_type() == SLIDER_MOVE) {
    SliderMoveEvent* slider_event = dynamic_cast<SliderMoveEvent*>(event);
    SEND(this, new ScrollEvent(slider_event->position));
    return;
  }

  for (auto& subwindow : subwindows) {
    subwindow->handle_event(event);
  }
}

Scrollbar::Scrollbar(Size size, Position pos, Color color,
                     uint16_t viewport_size, uint16_t scroll_block_size,
                     uint16_t step, bool horizontal)
    : RectWindow(size, pos, color) {
  Size button_size = {};

  Position bottom_button_pos = {};
  Position slider_default_position = {};
  Size slider_size = {};

  uint16_t slider_lower_boundary = 0;
  uint16_t slider_upper_boundary = 0;
  uint16_t slider_step = 0;

  /* Setting up pointers to members to unify further calculations */

  int16_t Position::*primary_axis = &Position::y;
  int16_t Position::*secondary_axis = &Position::x;

  uint16_t Size::*primary_size = &Size::height;
  uint16_t Size::*secondary_size = &Size::width;

  if (horizontal) {
    std::swap(primary_axis, secondary_axis);
    std::swap(primary_size, secondary_size);
  }

  /* calculating sizes and positions of scrollbar controls */

  button_size.*primary_size = size.*primary_size * SCROLLBAR_BUTTON_RATIO;
  button_size.*secondary_size = size.*secondary_size;

  slider_default_position.*primary_axis =
      pos.*primary_axis + button_size.*primary_size;
  slider_default_position.*secondary_axis = pos.*secondary_axis;

  bottom_button_pos.*primary_axis =
      pos.*primary_axis + size.*primary_size * (1 - SCROLLBAR_BUTTON_RATIO);
  bottom_button_pos.*secondary_axis = pos.*secondary_axis;

  slider_lower_boundary = slider_default_position.*primary_axis;
  slider_upper_boundary = bottom_button_pos.*primary_axis;

  slider_size.*primary_size = static_cast<float>(viewport_size) /
                              scroll_block_size * size.*primary_size *
                              (1 - 2 * SCROLLBAR_BUTTON_RATIO);
  slider_size.*secondary_size = size.*secondary_size;
  slider_step =
      static_cast<float>(slider_size.*primary_size) / viewport_size * step;

  Color controls_colors = color - CONTROLS_COLOR_DELTA;

  /* creating scrollbar controls */
  std::unique_ptr<Window> top_button(
      new RectButton(button_size, pos, controls_colors, UP));
  std::unique_ptr<Window> bottom_button(
      new RectButton(button_size, bottom_button_pos, controls_colors, DOWN));
  std::unique_ptr<Window> slider(new Slider(
      slider_size, slider_default_position, controls_colors,
      slider_lower_boundary, slider_upper_boundary, slider_step, horizontal));

  SUBSCRIBE(top_button.get(), slider.get());
  SUBSCRIBE(bottom_button.get(), slider.get());
  SUBSCRIBE(slider.get(), this);

  add_child_window(top_button);
  add_child_window(bottom_button);
  add_child_window(slider);
}

/*---------------------------------------*/
/*             ScrollableWindow          */
/*---------------------------------------*/
ScrollableWindow::ScrollableWindow(Size viewport_size,
                                   Size inner_container_size, Position pos,
                                   Color bg_color)
    : RectWindow(viewport_size, pos, bg_color),
      inner_container_size(inner_container_size),
      offset_x(0),
      offset_y(0) {}

void ScrollableWindow::render() {
  Renderer::draw_rectangle(size, pos, color);
  Renderer::init_offscreen_target(size, pos);
  RenderWindow::render();
  Renderer::flush_offscreen_target();
}

void ScrollableWindow::handle_event(Event* event) {
  assert(event != nullptr);

  switch (event->get_type()) {
    case SCROLL: {
      auto scroll_event = dynamic_cast<ScrollEvent*>(event);
      offset_y = -offset_y - (inner_container_size.height - size.height) *
                                 scroll_event->position;
      printf("Offset is %f\n", offset_y);
      fflush(stdout);
    }
  }

  for (auto& subwindow : subwindows) {
    auto window = dynamic_cast<RenderWindow*>(subwindow.get());
    window->move_children(offset_x, offset_y);
  }
}

/*---------------------------------------*/
/*                 Canvas                */
/*---------------------------------------*/
Canvas::Canvas(Size size, Position pos, Color color)
    : RectWindow(size, pos, color), img(size, color) {}

void Canvas::onMousePress(MouseButtonEvent* event) {
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    InstrumentManager::start_applying(event->pos);
  }
}

void Canvas::onMouseRelease(MouseButtonEvent* event) {
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    InstrumentManager::stop_applying();
  }
}

void Canvas::onMouseMove(MouseMoveEvent* event) {
  if (InstrumentManager::is_applying() && is_point_inside(event->pos)) {
    InstrumentManager::apply(img, event->pos);
  }
}

void Canvas::load_from_file(const char* filename) {
  img = std::move(Renderer::load_image(filename));
}

void Canvas::save_to_file(const char* filename) {
  Renderer::save_image(img, filename);
}

void Canvas::render() { Renderer::draw_image(pos, img); }

void Canvas::handle_event(Event* event) {
  assert(event != nullptr);

  switch (event->get_type()) {
    case MOUSE_BUTTON: {
      handle_mouse_button_event(event);
      break;
    }

    case MOUSE_MOVE: {
      auto move_event = dynamic_cast<MouseMoveEvent*>(event);
      onMouseMove(move_event);
      break;
    }
  }
}

/*---------------------------------------*/
/*                 Sprite                */
/*---------------------------------------*/

Sprite::Sprite(Texture text, Position pos) : texture(text), pos(pos) {}
void Sprite::render() { Renderer::draw_sprite(texture, pos); }

void Sprite::handle_event(Event* event) {}

/*---------------------------------------*/
/*                 HUESelector           */
/*---------------------------------------*/

HUEselector::HUEselector(Size size, Position pos) {
  auto canvas = new Canvas(size, pos, Color(255, 255, 255));
  float scale = size.width / 360;

  float r = 0;
  float g = 0;
  float b = 0;

  float h = 0;
  float s = 1;
  float v = 1;

  for (int i = 0; i < 360; ++i) {
    h = i;
    HSVtoRGB(r, g, b, h, s, v);

    Color cur_color = Color(r * 255, g * 255, b * 255);

    for (int j = 0; j < size.height; ++j) {
      canvas->img.setPixel(i * scale, j, cur_color);
    }
  }

  std::unique_ptr<Window> canvas_ptr(canvas);
  this->add_child_window(canvas_ptr);
}

void HUEselector::handle_event(Event* event) {
  assert(event != nullptr);

  switch (event->get_type()) {
    case SCROLL: {
      fflush(stdout);
      break;
    }
  }
}

/*---------------------------------------*/
/*                 SVelector             */
/*---------------------------------------*/
SVselector::SVselector(Size size, Position pos) {
  auto canvas = new Canvas(size, pos, Color(255, 255, 255));
  // value - y axis
  // sat - x axis

  float r = 0;
  float g = 0;
  float b = 0;

  float h = 0;
  float s = 1;
  float v = 1;

  for (int x = 0; x < size.width; ++x) {
    for (int y = 0; y < size.height; ++y) {
      s = static_cast<float>(x) / size.width;
      v = 1.f - static_cast<float>(y) / size.height;

      HSVtoRGB(r, g, b, h, s, v);

      Color cur_color = Color(r * 255, g * 255, b * 255);

      canvas->img.setPixel(x, y, cur_color);
    }
  }

  std::unique_ptr<Window> canvas_ptr(canvas);
  this->add_child_window(canvas_ptr);
}

void SVselector::handle_event(Event* event) {}
