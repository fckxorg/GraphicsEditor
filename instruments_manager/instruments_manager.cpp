#include "instruments_manager.hpp"

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
  float k =
      static_cast<float>(point.y - last_point.y) / (point.x - last_point.x);
  float b = point.y - k * point.x;

  for (int x = std::min(point.x, last_point.x);
       x <= std::max(point.x, last_point.x); x += 1) {
    canvas.setPixel(x, k * x + b, Color(0, 0, 0));
  }

  k = static_cast<float>(point.x - last_point.x) / (point.y - last_point.y);
  b = point.x - k * point.y;

  for (int y = std::min(point.y, last_point.y);
       y <= std::max(point.y, last_point.y); y += 1) {
    canvas.setPixel(k * y + b, y, Color(0, 0, 0));
  }
}

Eraser::Eraser() {
  set_color(Color(255, 255, 255));
  set_thickness(1);
}

bool InstrumentManager::application_started = false;
Position InstrumentManager::last_point = Position(0, 0);
std::vector<std::unique_ptr<AbstractInstrument>> InstrumentManager::instruments(COUNT);
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

