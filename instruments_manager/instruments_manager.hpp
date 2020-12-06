#ifndef INSTRUMENT_MANAGER
#define INSTRUMENT_MANAGER

#include <cstdarg>
#include <memory>
#include <random>
#include <vector>
#include <filesystem>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"
#include "../sfml_engine/sfml_engine.hpp"
#include "../subscription_manager/subscription_manager.hpp"
#include "../window_base/window_base.hpp"

class ToolbarListener : public Window {
 public:
  void handle_event(Event* event) override;
  void render() override;
};

enum INSTRUMENTS {
  ERASER,
  PENCIL,
  BRUSH,
  DROPPER,
  SPRAY,
  CLEAR,
  RECT_INSTRUMENT,
  ELLIPSE_INSTRUMENT,
  COUNT
};

class AbstractInstrument {
 public:
  virtual void init(Position pos);
  virtual void deinit(Image& canvas, Color color);
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) = 0;

  virtual ~AbstractInstrument();
};

class AbstractShapeInstrument : public AbstractInstrument {
 private:
    void default_axis_preparation(int16_t Position::* pos_axis,int16_t Size::* size_axis);
 protected:
  DelayedRenderData render_data;
  virtual void prepare_render_data();
  virtual void clear_render_data();
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
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

class Spray : public AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class Dropper : public AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class Clear : public AbstractInstrument {
 public:
  virtual void apply(Image& canvas, Position point, Position last_point,
                     Color color, uint8_t thickness) override;
};

class Rect : public AbstractShapeInstrument {
 public:
  virtual void init(Position pos) override;
  virtual void deinit(Image& canvas, Color color) override;
  friend class InstrumentManager;
};

class Ellipse : public AbstractShapeInstrument {
 public:
  virtual void init(Position pos) override;
  virtual void deinit(Image& canvas, Color color) override;
  friend class InstrumentManager;
};

class InstrumentManager {
 private:
  static bool application_started;
  static Position last_point;
  static std::vector<std::unique_ptr<AbstractInstrument>> instruments;
  static std::vector<PluginInfo> plugins_info;
  static int current_instrument;

  static uint8_t thickness;
  static Color color;

  static void get_plugins();

 public:
  static void init();

  static void start_applying(Position pos);

  static void stop_applying(Image& canvas);

  static void apply(Image& canvas, Position pos);

  static bool is_applying();

  static void set_instrument(uint8_t instrument);
  static void set_color(Color color);
  static void set_thickness(uint8_t thickness);

  static Color get_color();
};

#endif
