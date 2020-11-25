#include "instruments_manager.hpp"

#include <stdio.h>

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

void AbstractInstrument::set_color(Color color) { this->color = color; }

void AbstractInstrument::set_thickness(uint8_t thickness) {
  this->thickness = thickness;
}

AbstractInstrument::~AbstractInstrument() = default;

Pencil::Pencil() {
  set_color(Color(0, 0, 0));
  set_thickness(1);
}

void Pencil::apply(Image& canvas, Position point, Position last_point) {
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

Eraser::Eraser() {
  Pencil::set_color(Color(255, 255, 255));
  Pencil::set_thickness(5);
}

bool InstrumentManager::application_started = false;
Position InstrumentManager::last_point = Position(0, 0);
std::vector<std::unique_ptr<AbstractInstrument>> InstrumentManager::instruments(
    COUNT);
int InstrumentManager::current_instrument = PENCIL;

void InstrumentManager::init() {
  instruments[ERASER] =
      std::move(std::unique_ptr<AbstractInstrument>(new Eraser()));
  instruments[PENCIL] =
      std::move(std::unique_ptr<AbstractInstrument>(new Pencil()));
}

void InstrumentManager::start_applying(Position pos) {
  application_started = true;
  last_point = pos;
}

void InstrumentManager::stop_applying() { application_started = false; }

void InstrumentManager::apply(Image& canvas, Position pos) {
  switch (current_instrument) {
    case PENCIL: {
      dynamic_cast<Pencil*>(instruments[PENCIL].get())
          ->apply(canvas, pos, last_point);
      break;
    }

    case ERASER: {
      dynamic_cast<Eraser*>(instruments[ERASER].get())
          ->apply(canvas, pos, last_point);
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
  instruments[current_instrument]->set_color(color);
}

void InstrumentManager::set_thickness(uint8_t thickness) {
  instruments[current_instrument]->set_thickness(thickness);
}

