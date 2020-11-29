#ifndef INSTRUMENT_MANAGER
#define INSTRUMENT_MANAGER

#include <cstdarg>
#include <memory>
#include <vector>

#include "../data_classes/data_classes.hpp"
#include "../window_base/window_base.hpp"

class ToolbarListener : public Window {
 public:
  void handle_event(Event* event) override;
  void render() override;
};

enum INSTRUMENTS { ERASER, PENCIL, BRUSH, DROPPER, COUNT };

class AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) = 0;

  virtual ~AbstractInstrument();
};

class Pencil : public AbstractInstrument {
 public:
  Pencil();
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class Eraser : public Pencil {
 public:
  Eraser();
};

class Brush : public AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class Dropper : public AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class InstrumentManager {
 private:
  static bool application_started;
  static Position last_point;
  static std::vector<std::unique_ptr<AbstractInstrument>> instruments;
  static int current_instrument;

  static uint8_t thickness;
  static Color color;

 public:
  static void init();

  static void start_applying(Position pos);

  static void stop_applying();

  static void apply(Image& canvas, Position pos);

  static bool is_applying();

  static void set_instrument(uint8_t instrument);
  static void set_color(Color color);
  static void set_thickness(uint8_t thickness);
};

#endif
