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

enum INSTRUMENTS { ERASER, PENCIL, COUNT };

class AbstractInstrument {
 protected:
  Color color;
  uint8_t thickness;

 public:
  void set_color(Color color);
  void set_thickness(uint8_t thickness);

  virtual ~AbstractInstrument();
};

class Pencil : public AbstractInstrument {
 public:
  Pencil();
  virtual void apply(Image& canvas, Position point, Position last_point);
};

class Eraser : public Pencil {
 public:
  Eraser();
};

class InstrumentManager {
 private:
  static bool application_started;
  static Position last_point;
  static std::vector<std::unique_ptr<AbstractInstrument>> instruments;
  static int current_instrument;

 public:
  static void init();

  static void start_applying(Position pos);

  static void stop_applying();

  static void apply(Image& canvas, Position pos);

  static bool is_applying();

  static void set_instrument(uint8_t instrument);
};

#endif
