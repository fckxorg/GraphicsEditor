#include "window.hpp"

#include <bits/stdint-uintn.h>

#include <cstdio>

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

  Position click_position = event->pos;
  if (!is_point_inside(click_position)) return;

  Color curr_color = RectWindow::get_color();

  uint8_t color_r_pressed = std::min(curr_color.r - PRESS_FADE_DELTA, 0);
  uint8_t color_g_pressed = std::min(curr_color.g - PRESS_FADE_DELTA, 0);
  uint8_t color_b_pressed = std::min(curr_color.b - PRESS_FADE_DELTA, 0);

  Color pressed_color =
      Color(color_r_pressed, color_g_pressed, color_b_pressed);
  this->color = pressed_color;

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
    this->upper_bound = std::max(upper_bound - size.width, 0);
  } else {
    this->upper_bound = std::max(upper_bound - size.height, 0);
  }
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
  if (horizontal) {
    this->pos = Position(
        std::max(static_cast<uint16_t>(pos.x - step), lower_bound), pos.y);
  } else {
    this->pos = Position(
        pos.x, std::max(static_cast<uint16_t>(pos.y - step), lower_bound));
  }

  last_mouse_pos = pos;
}

void Slider::onButtonDown() {
  if (horizontal) {
    this->pos = Position(
        std::min(static_cast<uint16_t>(pos.x + step), upper_bound), pos.y);
  } else {
    this->pos = Position(
        pos.x, std::min(static_cast<uint16_t>(pos.y + step), upper_bound));
  }

  last_mouse_pos = pos;
}

void Slider::onMousePress(MouseButtonEvent* event) {
  assert(event != nullptr);

  Position click_position = event->pos;
  if (!is_point_inside(click_position)) return;

  Color curr_color = RectWindow::get_color();

  uint8_t color_r_pressed = std::min(curr_color.r - PRESS_FADE_DELTA, 0);
  uint8_t color_g_pressed = std::min(curr_color.g - PRESS_FADE_DELTA, 0);
  uint8_t color_b_pressed = std::min(curr_color.b - PRESS_FADE_DELTA, 0);

  Color pressed_color =
      Color(color_r_pressed, color_g_pressed, color_b_pressed);
  this->color = pressed_color;
  pressed = true;
  last_mouse_pos = click_position;
}

void Slider::onMouseMove(MouseMoveEvent* event) {
  assert(event != nullptr);

  if (!pressed) return;
  Position mouse_position = event->pos;

  int slider_delta = 0;

  Position new_pos = {};

  if (horizontal) {
    new_pos = get_new_slider_pos(mouse_position, &Position::x, &Position::y,
                                 slider_delta);
  } else {
    new_pos = get_new_slider_pos(mouse_position, &Position::y, &Position::x,
                                 slider_delta);
  }

  SubscriptionManager::send_event(this, new ScrollEvent(slider_delta));

  last_mouse_pos = mouse_position;
  this->pos = new_pos;
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

float Scrollbar::get_scroll_ratio() { return scroll_ratio; }

void Scrollbar::handle_event(Event* event) {
  assert(event != nullptr);

  for (auto& subwindow : subwindows) {
    subwindow->handle_event(event);
  }
}

Scrollbar::Scrollbar(Size size, Position pos, Color color,
                     uint16_t viewport_size, uint16_t scroll_block_size,
                     uint16_t step, bool horizontal)
    : RectWindow(size, pos, color), horizontal(horizontal) {
  Size button_size = {};

  Position bottom_button_pos = {};
  Position slider_default_position = {};
  Size slider_size = {};

  uint16_t slider_lower_boundary = 0;
  uint16_t slider_upper_boundary = 0;

  if (horizontal) {
    button_size = {static_cast<uint8_t>(size.width * SCROLLBAR_BUTTON_RATIO),
                   size.height};
    slider_default_position = {static_cast<uint16_t>(pos.x + button_size.width),
                               pos.y};
    bottom_button_pos = {static_cast<uint16_t>(
                             pos.x + size.width * (1 - SCROLLBAR_BUTTON_RATIO)),
                         pos.y};

    slider_lower_boundary = slider_default_position.x;
    slider_upper_boundary = bottom_button_pos.x;
    slider_size = Size((static_cast<float>(viewport_size) / scroll_block_size) *
                           size.width * (1 - 2 * SCROLLBAR_BUTTON_RATIO),
                       size.height);
  } else {
    button_size = {size.width,
                   static_cast<uint8_t>(size.height * SCROLLBAR_BUTTON_RATIO)};
    bottom_button_pos = {
        pos.x, static_cast<uint16_t>(pos.y + size.height *
                                                 (1 - SCROLLBAR_BUTTON_RATIO))};
    slider_default_position = {
        pos.x, static_cast<uint16_t>(pos.y + button_size.height)};

    slider_lower_boundary = slider_default_position.y;
    slider_upper_boundary = bottom_button_pos.y;
    slider_size = Size(size.width,
                       (static_cast<float>(viewport_size) / scroll_block_size) *
                           size.height * (1 - 2 * SCROLLBAR_BUTTON_RATIO));
  }

  scroll_ratio =
      static_cast<float>(scroll_block_size) /
      static_cast<float>(slider_upper_boundary - slider_lower_boundary);

  if (scroll_ratio < 1) {
    scroll_ratio = 1;
  }

  uint8_t red_comp_color = color.r - CONTROLS_COLOR_DELTA;
  uint8_t green_comp_color = color.g - CONTROLS_COLOR_DELTA;
  uint8_t blue_comp_color = color.b - CONTROLS_COLOR_DELTA;

  Color controls_colors =
      Color(red_comp_color, green_comp_color, blue_comp_color);

  std::unique_ptr<Window> top_button(
      new RectButton(button_size, pos, controls_colors, UP));
  std::unique_ptr<Window> bottom_button(
      new RectButton(button_size, bottom_button_pos, controls_colors, DOWN));
  std::unique_ptr<Window> slider(
      new Slider(slider_size, slider_default_position, controls_colors,
                 slider_lower_boundary, slider_upper_boundary,
                 step / scroll_ratio, horizontal));

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

  scroll_ratio = dynamic_cast<Scrollbar*>(scrollbar.get())->get_scroll_ratio();
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
      offset -= scroll_event->delta * scroll_ratio;
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
