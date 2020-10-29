#include "window.hpp"

#include <bits/stdint-uintn.h>

/*---------------------------------------*/
/*         Interface Clickable           */
/*---------------------------------------*/

void InterfaceClickable::handle_mouse_button_event(Event* event) {
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

/*---------------------------------------*/
/*              RectButton               */
/*---------------------------------------*/

RectButton::RectButton() = default;
RectButton::~RectButton() = default;

RectButton::RectButton(Size size, Position pos, Color color)
    : RectWindow(size, pos, color), default_color(color) {}

void RectButton::render() { RectWindow::render(); }

void RectButton::onMousePress(MouseButtonEvent* event) {
    Position click_position = event->pos;
    if (!is_point_inside(click_position)) return;

    Color curr_color = RectWindow::get_color();

    uint8_t color_r_pressed = std::min(curr_color.r - 50, 0);
    uint8_t color_g_pressed = std::min(curr_color.g - 50, 0);
    uint8_t color_b_pressed = std::min(curr_color.b - 50, 0);

    Color pressed_color =
        Color(color_r_pressed, color_g_pressed, color_b_pressed);
    this->color = pressed_color;
}

void RectButton::onMouseRelease(MouseButtonEvent* event) {
    this->color = default_color;
}

void RectButton::handle_event(Event* event) {
    if (event->get_type() == SYSTEM_EVENT::MOUSE_BUTTON) {
        handle_mouse_button_event(event);
    }

    Window::handle_event(event);
}

bool RectWindow::is_point_inside(Position point) {
    if (point.x < pos.x || point.x > pos.x + size.width) return false;
    if (point.y < pos.y || point.y > pos.y + size.height) return false;

    return true;
}

/*---------------------------------------*/
/*              TextWindow               */
/*---------------------------------------*/

TextWindow::TextWindow() = default;
TextWindow::TextWindow(Text text, Position pos, Color bgcolor)
    : text(text), bgcolor(bgcolor) {
    set_pos(pos);
}

Text TextWindow::get_text() const { return text; }

void TextWindow::set_text(Text text) { this->text = text; }

Color TextWindow::get_bgcolor() const { return bgcolor; }

void TextWindow::set_bgcolor(Color bgcolor) { this->bgcolor = bgcolor; }

void TextWindow::render() {
    Renderer::draw_text(text, pos, bgcolor);
    RenderWindow::render();
}

/*---------------------------------------*/
/*                Slider                 */
/*---------------------------------------*/

Slider::Slider() = default;
Slider::~Slider() = default;
Slider::Slider(Size size, Position pos, Color color, uint16_t lower_bound,
               uint16_t upper_bound, bool horizontal)
    : RectWindow(size, pos, color),
      default_color(color),
      pressed(false),
      lower_bound(lower_bound),
      upper_bound(upper_bound),
      horizontal(horizontal),
      last_mouse_pos(pos) {}

void Slider::handle_event(Event* event) {
    switch (event->get_type()) {
        case MOUSE_BUTTON: {
            handle_mouse_button_event(event);
            break;
        }

        case MOUSE_MOVE:
            auto mouse_move_event = dynamic_cast<MouseMoveEvent*>(event);
            onMouseMove(mouse_move_event);
            break;
    }

    Window::handle_event(event);
}

void Slider::onMousePress(MouseButtonEvent* event) {
    Position click_position = event->pos;
    if (!is_point_inside(click_position)) return;

    Color curr_color = RectWindow::get_color();

    uint8_t color_r_pressed = std::min(curr_color.r - 50, 0);
    uint8_t color_g_pressed = std::min(curr_color.g - 50, 0);
    uint8_t color_b_pressed = std::min(curr_color.b - 50, 0);

    Color pressed_color =
        Color(color_r_pressed, color_g_pressed, color_b_pressed);
    this->color = pressed_color;
    pressed = true;
    last_mouse_pos = click_position;
}

void Slider::onMouseMove(MouseMoveEvent* event) {
    if (!pressed) return;
    Position mouse_position = event->pos;

    int delta_x = mouse_position.x - last_mouse_pos.x;
    int delta_y = mouse_position.y - last_mouse_pos.y;

    last_mouse_pos = mouse_position;

    Position new_pos = {};

    if (horizontal) {
        int adjusted_upper_bound = std::max(upper_bound - size.width, 0);
        new_pos.x = std::min(pos.x + delta_x, adjusted_upper_bound);
        new_pos.x = std::max(new_pos.x, lower_bound);

        new_pos.y = pos.y;
    } else {
        int adjusted_upper_bound = std::max(upper_bound - size.height, 0);
        new_pos.y = std::min(pos.y + delta_y, adjusted_upper_bound);
        new_pos.y = std::max(new_pos.y, lower_bound);

        new_pos.x = pos.x;
    }

    this->pos = new_pos;
}

void Slider::onMouseRelease(MouseButtonEvent* event) {
    this->color = default_color;
    pressed = false;
}

/*---------------------------------------*/
/*             Scrollbar                 */
/*---------------------------------------*/

Scrollbar::Scrollbar() = default;
Scrollbar::~Scrollbar() = default;

Scrollbar::Scrollbar(Size size, Position pos, Color color, bool horizontal)
    : RectWindow(size, pos, color), horizontal(horizontal) {
    Size button_size = {};
    Size slider_size = {};

    Position bottom_button_pos = {};
    Position slider_default_position = {};

    uint16_t slider_lower_boundary = 0;
    uint16_t slider_upper_boundary = 0;

    if (horizontal) {
        button_size = {static_cast<uint8_t>(size.width * 0.1), size.height};
        slider_size = {static_cast<uint8_t>(size.width * 0.3),
                       size.height};  // TODO adaptive size
        slider_default_position = {
            static_cast<uint16_t>(pos.x + button_size.width), pos.y};
        bottom_button_pos = {static_cast<uint16_t>(pos.x + size.width * 0.9),
                             pos.y};

        slider_lower_boundary = slider_default_position.x;
        slider_upper_boundary = bottom_button_pos.x;
    } else {
        button_size = {size.width, static_cast<uint8_t>(size.height * 0.1)};
        bottom_button_pos = {pos.x,
                             static_cast<uint16_t>(pos.y + size.height * 0.9)};
        slider_size = {
            size.width,
            static_cast<uint8_t>(size.height * 0.3)};  // TODO adaptive size
        slider_default_position = {
            pos.x, static_cast<uint16_t>(pos.y + button_size.height)};

        slider_lower_boundary = slider_default_position.y;
        slider_upper_boundary = bottom_button_pos.y;
    }

    uint8_t red_comp_color = color.r - 30;
    uint8_t green_comp_color = color.g - 30;
    uint8_t blue_comp_color = color.b - 30;

    Color controls_colors =
        Color(red_comp_color, green_comp_color, blue_comp_color);

    std::unique_ptr<Window> top_button(
        new RectButton(button_size, pos, controls_colors));
    std::unique_ptr<Window> bottom_button(
        new RectButton(button_size, bottom_button_pos, controls_colors));
    std::unique_ptr<Window> slider(
        new Slider(slider_size, slider_default_position, controls_colors,
                   slider_lower_boundary, slider_upper_boundary, horizontal));

    add_child_window(top_button);
    add_child_window(bottom_button);
    add_child_window(slider);
}
