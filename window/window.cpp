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
void RootWindow::handle_event(Event* event) {
  SubscriptionManager::send_event(this, event);
};

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

  SubscriptionManager::send_event(this, new ButtonPressEvent(value));
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

      SubscriptionManager::send_event(
          this, new ButtonPressEvent(button_press_event->value));
      break;
    }
  }
}

void Slider::onButtonUp() {
  move(-step);
  last_mouse_pos = pos;
}

void Slider::onButtonDown() {
  move(step);
  last_mouse_pos = pos;
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

  float position = static_cast<float>(pos.*primary_axis - lower_bound) /
                   static_cast<float>(upper_bound - lower_bound);

  SubscriptionManager::send_event(this, new ScrollEvent(position));

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

  uint16_t Position::*primary_axis = &Position::y;
  uint16_t Position::*secondary_axis = &Position::x;

  uint16_t Size::*primary_size = &Size::height;
  uint16_t Size::*secondary_size = &Size::width;

  if(horizontal) {
        std::swap(primary_axis, secondary_axis);
        std::swap(primary_size, secondary_size);
  }

  button_size.*primary_size = size.*primary_size * SCROLLBAR_BUTTON_RATIO;
  button_size.*secondary_size = size.*secondary_size;
  
  slider_default_position.*primary_axis = pos.*primary_axis + button_size.*primary_size;
  slider_default_position.*secondary_axis = pos.*secondary_axis;

  bottom_button_pos.*primary_axis = pos.*primary_axis + size.*primary_size * (1 - SCROLLBAR_BUTTON_RATIO);
  bottom_button_pos.*secondary_axis = pos.*secondary_axis;

  slider_lower_boundary = slider_default_position.*primary_axis;
  slider_upper_boundary = bottom_button_pos.*primary_axis;

  slider_size.*primary_size = static_cast<float>(viewport_size) / scroll_block_size * size.*primary_size * (1 - 2 * SCROLLBAR_BUTTON_RATIO);
  slider_size.*secondary_size = size.*secondary_size;

  Color controls_colors = color - CONTROLS_COLOR_DELTA;

  std::unique_ptr<Window> top_button(
      new RectButton(button_size, pos, controls_colors, UP));
  std::unique_ptr<Window> bottom_button(
      new RectButton(button_size, bottom_button_pos, controls_colors, DOWN));
  std::unique_ptr<Window> slider(new Slider(
      slider_size, slider_default_position, controls_colors,
      slider_lower_boundary, slider_upper_boundary, step, horizontal));

  SubscriptionManager::add_subscription(top_button.get(), slider.get());
  SubscriptionManager::add_subscription(bottom_button.get(), slider.get());

  slider_ptr = slider.get();

  add_child_window(top_button);
  add_child_window(bottom_button);
  add_child_window(slider);
}

/*---------------------------------------*/
/*             ScrollableText            */
/*---------------------------------------*/

ScrollableText::ScrollableText(Size viewport_size, Position pos, Color bg_color,
                               Text text)
    : RectWindow(viewport_size, pos, bg_color), text(text), offset(0) {
  uint16_t n_lines = ScrollableText::get_nlines();
  whole_block_height = n_lines * (text.character_size * text.line_spacing *
                                  LINESPACING_COEFF * text.character_size);

  std::unique_ptr<Window> scrollbar(new Scrollbar(
      Size(viewport_size.width * SCROLLBAR_SIZE_RATIO, viewport_size.height),
      Position(pos.x + viewport_size.width, pos.y), Color(245, 245, 245),
      viewport_size.height, whole_block_height, text.character_size, false));
  SubscriptionManager::add_subscription(
      dynamic_cast<Scrollbar*>(scrollbar.get())->slider_ptr, this);
  add_child_window(scrollbar);
}

void ScrollableText::render() {
  Renderer::draw_scrollable_text(text, size, pos, color, offset);
  RenderWindow::render();
}

void ScrollableText::handle_event(Event* event) {
  assert(event != nullptr);

  switch (event->get_type()) {
    case BUTTON_PRESSED: {
      auto button_press_event = dynamic_cast<ButtonPressEvent*>(event);
      if (button_press_event->value == UP) {
        onButtonUp();
      } else if (button_press_event->value == DOWN) {
        onButtonDown();
      }
      return;
    }
    case SCROLL: {
      auto scroll_event = dynamic_cast<ScrollEvent*>(event);
      offset = -(whole_block_height - size.height) * scroll_event->position;
    }
  }

  for (auto& subwindow : subwindows) {
    subwindow->handle_event(event);
  }
}

void ScrollableText::onButtonDown() {
  if (offset - text.character_size < -whole_block_height + size.height) {
    offset = -whole_block_height + size.height;
  }
  offset -= text.character_size;
}

void ScrollableText::onButtonUp() {
  if (offset + text.character_size > 0) {
    offset = 0;
  }
  offset += text.character_size;
}

uint16_t ScrollableText::get_nlines() {
  int16_t n_lines = 1;
  const char* cur = text.text;

  while (*cur != '\0') {
    if (*cur == '\n') {
      ++n_lines;
    }
    ++cur;
  }

  return n_lines;
}
