#ifndef INSTRUMENT_MANAGER
#define INSTRUMENT_MANAGER

#include "../data_classes/data_classes.hpp"

class InstrumentManager {
 private:
  static bool application_started;
  static Position last_point;

 public:
  static void start_applying(Position pos);

  static void stop_applying();

  static void apply(Image& canvas, Position pos);

  static bool is_applying();
};

#endif
