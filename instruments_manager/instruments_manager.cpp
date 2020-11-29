#include "instruments_manager.hpp"

const int MAX_THICKNESS = 40;

void ToolbarListener::handle_event(Event* event) {
  if (event->get_type() == BUTTON_PRESSED) {
    auto button_event = dynamic_cast<ButtonPressEvent*>(event);
    InstrumentManager::set_instrument(button_event->value);
  }
  if (event->get_type() == COLOR_CHANGED) {
    auto color_event = dynamic_cast<ColorChangedEvent*>(event);
    InstrumentManager::set_color(color_event->color);
  }

  if (event->get_type() == SLIDER_MOVE) {
    auto slider_event = dynamic_cast<SliderMoveEvent*>(event);
    InstrumentManager::set_thickness(slider_event->position * MAX_THICKNESS);
  }
}

void ToolbarListener::render() {}

AbstractInstrument::~AbstractInstrument() = default;

Pencil::Pencil() = default;

void Pencil::apply(Image& canvas, Position point, Position last_point,
                   Color color, uint8_t thickness) {
  int16_t Position::*primary_axis = nullptr;
  int16_t Position::*secondary_axis = nullptr;

  int16_t x_diff = abs(point.x - last_point.x);
  int16_t y_diff = abs(point.y - last_point.y);

  if (x_diff > y_diff) {
    primary_axis = &Position::x;
    secondary_axis = &Position::y;
  } else {
    primary_axis = &Position::y;
    secondary_axis = &Position::x;
  }

  // TODO splines
  // if there is not enough points for approximation (usually 4) you can
  // duplicate points
  float k =
      static_cast<float>(point.*secondary_axis - last_point.*secondary_axis) /
      (point.*primary_axis - last_point.*primary_axis);
  float b = point.*secondary_axis - k * point.*primary_axis;

  for (int x = std::min(point.*primary_axis, last_point.*primary_axis);
       x <= std::max(point.*primary_axis, last_point.*primary_axis); x += 1) {
    for (int i = -thickness; i <= thickness; ++i) {
      for (int j = -thickness; j <= thickness; ++j) {
        if (i * i + j * j < thickness * thickness) {
          if (x_diff > y_diff) {
            canvas.setPixel(std::max(x + i, 0), std::max(k * x + b + j, 0.f),
                            color);
          } else {
            canvas.setPixel(std::max(k * x + b + i, 0.f), std::max(x + j, 0),
                            color);
          }
        }
      }
    }
  }
}

Eraser::Eraser() = default;

void Brush::apply(Image& canvas, Position point, Position last_point,
                  Color color, uint8_t thickness) {
  int16_t Position::*primary_axis = nullptr;
  int16_t Position::*secondary_axis = nullptr;

  int16_t x_diff = abs(point.x - last_point.x);
  int16_t y_diff = abs(point.y - last_point.y);

  if (x_diff > y_diff) {
    primary_axis = &Position::x;
    secondary_axis = &Position::y;
  } else {
    primary_axis = &Position::y;
    secondary_axis = &Position::x;
  }

  // TODO splines
  // if there is not enough points for approximation (usually 4) you can
  // duplicate points
  float k =
      static_cast<float>(point.*secondary_axis - last_point.*secondary_axis) /
      (point.*primary_axis - last_point.*primary_axis);
  float b = point.*secondary_axis - k * point.*primary_axis;

  for (int x = std::min(point.*primary_axis, last_point.*primary_axis);
       x <= std::max(point.*primary_axis, last_point.*primary_axis); x += 1) {
    for (int i = -thickness; i <= thickness; ++i) {
      if (x_diff > y_diff) {
        canvas.setPixel(x, k * x + b + i, color);
      } else {
        canvas.setPixel(k * x + b, x + i, color);
      }
    }
  }
}

bool InstrumentManager::application_started = false;
Position InstrumentManager::last_point = Position(0, 0);
std::vector<std::unique_ptr<AbstractInstrument>> InstrumentManager::instruments(
    COUNT);
int InstrumentManager::current_instrument = PENCIL;
uint8_t InstrumentManager::thickness = 1;
Color InstrumentManager::color = Color(0, 0, 0);

void InstrumentManager::init() {
  instruments[ERASER] =
      std::move(std::unique_ptr<AbstractInstrument>(new Eraser()));
  instruments[PENCIL] =
      std::move(std::unique_ptr<AbstractInstrument>(new Pencil()));
  instruments[BRUSH] =
      std::move(std::unique_ptr<AbstractInstrument>(new Brush()));
}

void InstrumentManager::start_applying(Position pos) {
  application_started = true;
  last_point = pos;
}

void InstrumentManager::stop_applying() { application_started = false; }

void InstrumentManager::apply(Image& canvas, Position pos) {
  switch (current_instrument) {
    case ERASER: {
      instruments[current_instrument]->apply(canvas, pos, last_point,
                                             Color(255, 255, 255), thickness);
      break;
    }

    default: {
      instruments[current_instrument]->apply(canvas, pos, last_point, color,
                                             thickness);
      break;
    }
  }

  last_point = pos;
}

bool InstrumentManager::is_applying() { return application_started; }

void InstrumentManager::set_instrument(uint8_t instrument) {
  current_instrument = instrument;
}

void InstrumentManager::set_color(Color color) {
  InstrumentManager::color = color;
}

void InstrumentManager::set_thickness(uint8_t thickness) {
  InstrumentManager::thickness = thickness;
}

