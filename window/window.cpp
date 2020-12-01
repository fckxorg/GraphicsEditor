#include "window.hpp"

const uint8_t PRESS_FADE_DELTA = 50;
const uint8_t CONTROLS_COLOR_DELTA = 30;
const float SCROLLBAR_BUTTON_RATIO = 0.1;
const float LINESPACING_COEFF = 0.08;
const float SCROLLBAR_SIZE_RATIO = 0.07;
const float INPUTBOX_TEXT_OFFSET = 5;
const int16_t INPUTBOX_SAVE_DIALOG_OFFSET_X = 30;
const int16_t INPUTBOX_SAVE_DIALOG_OFFSET_Y = 530;

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
RenderWindow::~RenderWindow() {
  SubscriptionManager::unsubscribe_all(this);
  SubscriptionManager::unsubscribe_from_all(this);
}
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

Size RectWindow::get_size() { return this->size; }

void RectWindow::set_size(Size size) { this->size = size; }

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
}

void RectButton::onMouseRelease(MouseButtonEvent* event) {
  assert(event != nullptr);
  if (is_point_inside(event->pos)) {
    SEND(this, new ButtonPressEvent(value));
  }
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

void Slider::move_relative(float offset) {
  pos.*primary_axis = lower_bound + (upper_bound - lower_bound) * offset;
  pos.*primary_axis =
      std::max(pos.*primary_axis, static_cast<int16_t>(lower_bound));
  pos.*primary_axis =
      std::min(pos.*primary_axis, static_cast<int16_t>(upper_bound));
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

  int16_t Size::*primary_size = &Size::height;
  int16_t Size::*secondary_size = &Size::width;

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
      offset_y =
          -(inner_container_size.height - size.height) * scroll_event->position;
      for (auto& subwindow : subwindows) {
        auto window = dynamic_cast<RenderWindow*>(subwindow.get());
        window->set_pos(Position(offset_x, offset_y));
      }
    }
  }
}

/*---------------------------------------*/
/*                 Canvas                */
/*---------------------------------------*/
Canvas::Canvas(Size size, Position pos, Color color)
    : RectWindow(size, pos, color), img(size, color) {}

void Canvas::onMousePress(MouseButtonEvent* event) {
  if (!is_point_inside(event->pos)) return;
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    InstrumentManager::start_applying(event->pos);
    InstrumentManager::apply(img, event->pos);
  }
}

void Canvas::onMouseRelease(MouseButtonEvent* event) {
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    InstrumentManager::stop_applying(img);
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

    case BUTTON_PRESSED: {
      auto button_event = dynamic_cast<ButtonPressEvent*>(event);
      if (button_event->value == SAVE) {
        save_to_file("sketch.png");  // TODO dialog window
      }
    }
  }
}

/*---------------------------------------*/
/*                 Sprite                */
/*---------------------------------------*/

Sprite::Sprite(Texture text, Position pos) : texture(text) {
   RenderWindow::pos = pos;
}
void Sprite::render() { Renderer::draw_sprite(texture, pos); }

void Sprite::handle_event(Event* event) {}

/*---------------------------------------*/
/*                 HUESelector           */
/*---------------------------------------*/

HUEselector::HUEselector(Size size, Position pos) : size(size) {
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
    case SLIDER_MOVE: {
      auto slider_event = dynamic_cast<SliderMoveEvent*>(event);
      float new_hue = size.width * slider_event->position;
      SEND(this, new HueChangedEvent(new_hue));
      break;
    }
  }
}

/*---------------------------------------*/
/*                 SVelector             */
/*---------------------------------------*/
SVselector::SVselector(Size size, Position pos) : size(size) {
  canvas = new Canvas(size, pos, Color(255, 255, 255));
  redraw_canvas(0);
  std::unique_ptr<Window> canvas_ptr(canvas);
  this->add_child_window(canvas_ptr);
}

void SVselector::redraw_canvas(float hue) {
  // value - y axis
  // sat - x axis

  float r = 0;
  float g = 0;
  float b = 0;

  float h = hue;
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
}

void SVselector::handle_event(Event* event) {
  switch (event->get_type()) {
    case HUE_CHANGED: {
      auto hue_event = dynamic_cast<HueChangedEvent*>(event);
      redraw_canvas(hue_event->hue);
      break;
    }
    case FADER_MOVE: {
      auto fader_event = dynamic_cast<FaderMoveEvent*>(event);
      Color selected_color = canvas->img.getPixel(
          fader_event->pos_x * size.width, fader_event->pos_y * size.height);

      SEND(this, new ColorChangedEvent(selected_color));
      break;
    }
  }
}

/*---------------------------------------*/
/*                 HueSlider             */
/*---------------------------------------*/

void HueSlider::handle_event(Event* event) {
  Slider::handle_event(event);
  if (event->get_type() == DROPPER_APPLIED) {
    Color new_color = dynamic_cast<DropperEvent*>(event)->color;
    float r_prep = static_cast<float>(new_color.r) / 255;
    float g_prep = static_cast<float>(new_color.g) / 255;
    float b_prep = static_cast<float>(new_color.b) / 255;

    float h = 0;
    float s = 0;
    float v = 0;

    RGBtoHSV(r_prep, g_prep, b_prep, h, s, v);
    move_relative(h / 360);

    SEND(this, new SliderMoveEvent(get_relative_pos()));
  }
}

HueSlider::HueSlider(Size size, Position pos, Color color, uint16_t lower_bound,
                     uint16_t upper_bound, uint16_t step, bool horizontal)
    : Slider(size, pos, color, lower_bound, upper_bound, step, horizontal){};

/*---------------------------------------*/
/*                 Fader                 */
/*---------------------------------------*/

Fader::Fader(Size size, Position pos, Color color, Position lower_bound,
             Position upper_bound)
    : RectWindow(size, pos, color),
      lower_bound(lower_bound),
      upper_bound(upper_bound) {}

void Fader::handle_event(Event* event) {
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
  }
}

void Fader::onMousePress(MouseButtonEvent* event) {
  if (event->pos.x < lower_bound.x || event->pos.y < lower_bound.y) return;
  if (event->pos.x > upper_bound.x || event->pos.y > upper_bound.y) return;

  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    pressed = true;
    this->pos = event->pos;
    float pos_x = static_cast<float>(pos.x - lower_bound.x) /
                  (upper_bound.x - lower_bound.x);
    float pos_y = static_cast<float>(pos.y - lower_bound.y) /
                  (upper_bound.y - lower_bound.y);

    SEND(this, new FaderMoveEvent(pos_x, pos_y));
  }
}

void Fader::onMouseRelease(MouseButtonEvent* event) {
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    pressed = false;
  }
}

void Fader::onMouseMove(MouseMoveEvent* event) {
  if (event->pos.x < lower_bound.x || event->pos.y < lower_bound.y) return;
  if (event->pos.x > upper_bound.x || event->pos.y > upper_bound.y) return;
  if (!pressed) return;

  this->pos = event->pos;
  float pos_x = static_cast<float>(pos.x - lower_bound.x) /
                (upper_bound.x - lower_bound.x);
  float pos_y = static_cast<float>(pos.y - lower_bound.y) /
                (upper_bound.y - lower_bound.y);

  SEND(this, new FaderMoveEvent(pos_x, pos_y));
}

void Fader::render() {
  Renderer::draw_rectangle(size, pos, color);
  Size inline_size = Size(size.width - 4, size.height - 4);
  Position inline_pos = Position(pos.x + 2, pos.y + 2);
  Renderer::draw_rectangle(inline_size, inline_pos, Color(255, 255, 255));
}

/*---------------------------------------*/
/*               SVFader                 */
/*---------------------------------------*/

SVFader::SVFader(Size size, Position pos, Color color, Position lower_bound,
                 Position upper_bound)
    : Fader(size, pos, color, lower_bound, upper_bound) {}

void SVFader::handle_event(Event* event) {
  Fader::handle_event(event);

  switch (event->get_type()) {
    case DROPPER_APPLIED: {
      Color new_color = dynamic_cast<DropperEvent*>(event)->color;
      float r_prep = static_cast<float>(new_color.r) / 255;
      float g_prep = static_cast<float>(new_color.g) / 255;
      float b_prep = static_cast<float>(new_color.b) / 255;

      float h = 0;
      float s = 0;
      float v = 0;

      RGBtoHSV(r_prep, g_prep, b_prep, h, s, v);
      float pos_x = s * (upper_bound.x - lower_bound.x);
      float pos_y = (1.f - v) * (upper_bound.y - lower_bound.y);

      pos.x = lower_bound.x + pos_x;
      pos.y = lower_bound.y + pos_y;

      SEND(this, new FaderMoveEvent(s, 1.f - v));
      break;
    }
    case HUE_CHANGED: {
      auto hue_event = dynamic_cast<HueChangedEvent*>(event);

      float pos_x = static_cast<float>(pos.x - lower_bound.x) /
                    (upper_bound.x - lower_bound.x);
      float pos_y = static_cast<float>(pos.y - lower_bound.y) /
                    (upper_bound.y - lower_bound.y);
      SEND(this, new FaderMoveEvent(pos_x, pos_y));
      break;
    }
  }
}

/*---------------------------------------*/
/*              Inputbox                 */
/*---------------------------------------*/
Inputbox::Inputbox(Size size, Position pos, Color color,
                   uint16_t character_size, const char* font_path,
                   Color text_color)
    : RectWindow(size, pos, color), active(false) {
  input_text.bg_color = color;
  input_text.character_size = character_size;
  input_text.font_path = font_path;
  input_text.line_spacing = 1;
  input_text.color = text_color;
}

void Inputbox::handle_event(Event* event) {
  switch (event->get_type()) {
    case KEY_PRESSED: {
      if (active) {
        auto key_event = dynamic_cast<KeyPressedEvent*>(event);

        if (key_event->key < 26) {
          char symbol = key_event->shift ? 'A' : 'a';
          input_value.push_back(key_event->key + symbol);
        }

        switch (key_event->key) {
          case Backspace: {
            if (!input_value.empty()) {
              input_value.pop_back();
            }
            break;
          }
          case Space: {
            input_value.push_back(' ');
            break;
          }
          case Return: {
            active = false;
            break;
          }
        }
      }
      break;
    }
    case MOUSE_BUTTON: {
      handle_mouse_button_event(event);
      break;
    }
  }
}

void Inputbox::onMousePress(MouseButtonEvent* event) {
  if (event->button == MouseButtonEvent::MouseButton::LEFT) {
    active = true;
  }
}

void Inputbox::onMouseRelease(MouseButtonEvent* event) {}

void Inputbox::render() {
  Renderer::draw_rectangle(size, pos, color);
  input_text.text = input_value.data();

  Position text_pos =
      Position(pos.x + INPUTBOX_TEXT_OFFSET, pos.y + INPUTBOX_TEXT_OFFSET);
  Renderer::draw_text(input_text, text_pos);
}

/*---------------------------------------*/
/*              DialogWindow             */
/*---------------------------------------*/
DialogWindow::DialogWindow(Size size, Position pos, Color color,
                           Color outline_color, int16_t outline_thickness,
                           Window* creator)
    : RectWindow(size, pos, color),
      outline_color(outline_color),
      outline_thickness(outline_thickness),
      creator(creator) {
  SubscriptionManager::init_new_layer();
  SUBSCRIBE(this, creator);
}

DialogWindow::~DialogWindow() { SubscriptionManager::cleanup(); }

void DialogWindow::render() {
  Position outline_pos =
      Position(pos.x - outline_thickness, pos.y - outline_thickness);
  Size outline_size = Size(size.width + 2 * outline_thickness,
                           size.height + 2 * outline_thickness);
  Renderer::draw_rectangle(outline_size, outline_pos, outline_color);
  Renderer::draw_rectangle(size, pos, color);
  RenderWindow::render();
}

/*---------------------------------------*/
/*          DialogSaveWindow             */
/*---------------------------------------*/

DialogSaveWindow::DialogSaveWindow(Size size, Position pos, Color color,
                                   Color outline_color,
                                   int16_t outline_thickness, Window* creator)
    : DialogWindow(size, pos, color, outline_color, outline_thickness,
                   creator) {
  Size inputbox_size = Size(610, 30);
  Position inputbox_pos = Position(pos.x + INPUTBOX_SAVE_DIALOG_OFFSET_X + 5,
                                   pos.y + INPUTBOX_SAVE_DIALOG_OFFSET_Y + 5);
  Size inputbox_outline_size =
      Size(inputbox_size.width + 10, inputbox_size.height + 10);
  Position inputbox_outline_pos =
      Position(inputbox_pos.x - 5, inputbox_pos.y - 5);
#include "../layouts/file_dialog_window.hpp"
};

/*---------------------------------------*/
/*              SaveButton               */
/*---------------------------------------*/

SaveButton::SaveButton(Size size, Position pos, Color color)
    : RectButton(size, pos, color) {}

void SaveButton::handle_event(Event* event) {
  RectButton::handle_event(event);
  if (event->get_type() == DIALOG_END) {
    subwindows.pop_back();
  }
}

void SaveButton::onMouseRelease(MouseButtonEvent* event) {
  RectButton::onMouseRelease(event);

  if (!is_point_inside(event->pos)) return;
  CREATE(dialog_window, DialogSaveWindow, Size(800, 600), Position(560, 240),
         Color(212, 212, 212), Color(80, 90, 91), 5, this);
  ADOPT(this, dialog_window);
}

/*---------------------------------------*/
/*            DialogEndButton            */
/*---------------------------------------*/
DialogEndButton::DialogEndButton(Size size, Position pos, Color color)
    : RectButton(size, pos, color) {}

void DialogEndButton::onMouseRelease(MouseButtonEvent* event) {
  RectButton::onMouseRelease(event);
  if (!is_point_inside(event->pos)) return;

  EventQueue::add_event(new Event(DIALOG_END));
}

/*---------------------------------------*/
/*             DirectoryEntry            */
/*---------------------------------------*/

DirectoryEntry::DirectoryEntry(Size size, Position pos, Color color, Text text,
                               const std::string& name, const char* icon_path,
                               int idx)
    : RectButton(size, pos, color, idx),
      name(name),
      icon_path(icon_path),
      text(text) {}

void DirectoryEntry::render() {
  RectButton::render();
  Renderer::draw_sprite(Texture(icon_path, Size(size.height, size.height)),
                        pos);
  text.text = name.data();
  Renderer::draw_text(text, Position(pos.x + size.height, pos.y));
}
