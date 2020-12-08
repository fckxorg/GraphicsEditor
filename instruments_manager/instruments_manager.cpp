#include "instruments_manager.hpp"

const int MAX_THICKNESS = 40;
const int SPRAY_DENSITY = 20;

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

void AbstractInstrument::init(Position pos) {}
void AbstractInstrument::deinit(Image& canvas, Color color) {}

void AbstractShapeInstrument::default_axis_preparation(
    int16_t Position::*pos_axis, int16_t Size::*size_axis) {
  if (render_data.size.*size_axis < 0) {
    render_data.pos.*pos_axis += render_data.size.*size_axis;
    render_data.size.*size_axis = -(render_data.size.*size_axis);
  }
}

void AbstractShapeInstrument::prepare_render_data() {
  default_axis_preparation(&Position::x, &Size::width);
  default_axis_preparation(&Position::y, &Size::height);
}

void AbstractShapeInstrument::clear_render_data() {
  render_data.pos = Position(-1, -1);
  render_data.size = Size(0, 0);
  Renderer::remove_delayed();
}

void AbstractShapeInstrument::apply(Image& canvas, Position point,
                                    Position last_point, Color color,
                                    uint8_t thickness) {
  render_data.size.width = point.x - render_data.pos.x;
  render_data.size.height = point.y - render_data.pos.y;
  render_data.color = color;
  Renderer::add_delayed(render_data);
}
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

void Dropper::apply(Image& canvas, Position point, Position last_point,
                    Color color, uint8_t thickness) {
  Color pixel = canvas.getPixel(point.x, point.y);
  SEND(SubscriptionManager::get_system_event_sender(), new DropperEvent(pixel));
}

void Spray::apply(Image& canvas, Position point, Position last_point,
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

  std::minstd_rand randomizer;

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
        bool not_colored = randomizer() % SPRAY_DENSITY;
        if (i * i + j * j < thickness * thickness && !not_colored) {
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

void Clear::apply(Image& canvas, Position point, Position last_point,
                  Color color, uint8_t thickness) {
  for (int x = 0; x < canvas.get_size().width; ++x) {
    for (int y = 0; y < canvas.get_size().height; ++y) {
      canvas.setPixel(x, y, color);
    }
  }
}

void Rect::init(Position pos) {
  render_data.pos = pos;
  render_data.type = RECT;
}

void Rect::deinit(Image& canvas, Color color) {
  prepare_render_data();

  for (int x = 0; x < render_data.size.width; ++x) {
    for (int y = 0; y < render_data.size.height; ++y) {
      canvas.setPixel(x + render_data.pos.x, y + render_data.pos.y, color);
    }
  }

  clear_render_data();
}

void Ellipse::init(Position pos) {
  render_data.pos = pos;
  render_data.type = ELLIPSE;
}

void Ellipse::deinit(Image& canvas, Color color) {
  prepare_render_data();

  float radius_hor = static_cast<float>(render_data.size.width) / 2;
  float radius_vert = static_cast<float>(render_data.size.height) / 2;

  for (int x = 0; x < render_data.size.width; ++x) {
    for (int y = 0; y < render_data.size.height; ++y) {
      float x_eq_part =
          (x - radius_hor) * (x - radius_hor) / (radius_hor * radius_hor);
      float y_eq_part =
          (y - radius_vert) * (y - radius_vert) / (radius_vert * radius_vert);
      if (x_eq_part + y_eq_part <= 1) {
        canvas.setPixel(x + render_data.pos.x, y + render_data.pos.y, color);
      }
    }
  }

  clear_render_data();
}


std::vector<std::unique_ptr<AbstractInstrument>> InstrumentManager::instruments(
    COUNT);
std::vector<void*> InstrumentManager::handles;
std::vector<PluginAPI::Plugin*> InstrumentManager::plugins;

bool InstrumentManager::application_started = false;
Position InstrumentManager::last_point = Position(-1, -1);
std::vector<PluginInfo> InstrumentManager::plugins_info;
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
  instruments[DROPPER] =
      std::move(std::unique_ptr<AbstractInstrument>(new Dropper()));
  instruments[SPRAY] =
      std::move(std::unique_ptr<AbstractInstrument>(new Spray()));
  instruments[CLEAR] =
      std::move(std::unique_ptr<AbstractInstrument>(new Clear()));
  instruments[RECT_INSTRUMENT] =
      std::move(std::unique_ptr<AbstractInstrument>(new Rect()));
  instruments[ELLIPSE_INSTRUMENT] =
      std::move(std::unique_ptr<AbstractInstrument>(new Ellipse()));

  get_plugins();
  EventQueue::add_event(new Event(LOAD_PLUGINS));
}

void InstrumentManager::start_applying(Position pos) {
  application_started = true;
  last_point.x = pos.x + 1;
  last_point.y = pos.y;

  instruments[current_instrument]->init(pos);
}

void InstrumentManager::stop_applying(Image& canvas) {
  application_started = false;
  instruments[current_instrument]->deinit(canvas, color);
}

void InstrumentManager::apply(Image& canvas, Position pos) {
  switch (current_instrument) {
    case ERASER: {
      instruments[current_instrument]->apply(canvas, pos, last_point,
                                             Color(255, 255, 255), thickness);
      break;
    }

    case CLEAR: {
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

Color InstrumentManager::get_color() { return InstrumentManager::color; }

void InstrumentManager::set_thickness(uint8_t thickness) {
  InstrumentManager::thickness = thickness;
}

void InstrumentManager::get_plugins() {
  auto plugins_path = std::filesystem::current_path();
  plugins_path /= "plugins";

  for (auto& plugin_entry : std::filesystem::directory_iterator(plugins_path)) {
    if (plugin_entry.is_directory()) {
      PluginInfo current_plugin_info;

      for (auto& plugin_asset :
           std::filesystem::directory_iterator(plugin_entry)) {
        auto filename = plugin_asset.path().filename();

        if (filename.string().starts_with("icon")) {
          current_plugin_info.icon_path = plugin_asset.path().string();
        } else if (filename.string().ends_with(".so")) {
          current_plugin_info.lib_path = plugin_asset.path().string();
        }
      }

      plugins_info.push_back(current_plugin_info);
    }
  }

  for (auto& info_entry : plugins_info) {
    printf("Found plugin %s %s\n", info_entry.icon_path.data(),
           info_entry.lib_path.data());
  }
}

void InstrumentManager::load_plugins() {
    for(auto& plugin : plugins_info) {
        void* handle = dlopen(plugin.lib_path.data(), RTLD_NOW);
        dlclose(handle);
    }   
}

