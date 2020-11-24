#include "instruments_manager.hpp"

bool InstrumentManager::application_started = false;
Position InstrumentManager::last_point = Position(0, 0);

void InstrumentManager::start_applying(Position pos) {
  application_started = true;
  last_point = pos;
}

void InstrumentManager::stop_applying() { application_started = false; }

void InstrumentManager::apply(Image& canvas, Position pos) {
  float k = static_cast<float>(pos.y - last_point.y) / (pos.x - last_point.x);
  float b = pos.y - k * pos.x;

  for (int x = std::min(pos.x, last_point.x);
       x <= std::max(pos.x, last_point.x); x += 1) {
    canvas.setPixel(x, k * x + b, Color(0, 0, 0));
  }

  k = static_cast<float>(pos.x - last_point.x) / (pos.y - last_point.y);
  b = pos.x - k * pos.y;

  for (int y = std::min(pos.y, last_point.y);
       y <= std::max(pos.y, last_point.y); y += 1) {
    canvas.setPixel(k * y + b, y, Color(0, 0, 0));
  }

  last_point = pos;
}

bool InstrumentManager::is_applying() { return application_started; }

