#ifndef SFML_ENGINE_HPP
#define SFML_ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cassert>
#include <stack>
#include <unordered_map>
#include <cmath>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"

enum DELAYED_RENDER_TYPES { RECT, ELLIPSE };

struct DelayedRenderData {
  int type;
  Size size;
  Position pos;
  Color color;
};

struct OffscreenRenderData {
  sf::Sprite* sprite;
  sf::RenderTexture* texture;

  OffscreenRenderData();

  void release();
};

class Renderer {
 private:
  static sf::RenderWindow window;

  static DelayedRenderData delayed_render;
  static bool has_delayed;

  static std::unordered_map<const char*, sf::Font> fonts;
  static std::unordered_map<const char*, sf::Texture> textures;
  static std::vector<OffscreenRenderData> offscreen_resources;
  static std::stack<OffscreenRenderData> offscreen_render_stack;
  static std::stack<Position> global_offsets;

  static sf::RenderTarget* get_target();

  static sf::Text get_sfml_text(Text text);
  static sf::Image get_sfml_image(
      const std::vector<std::vector<Color>>& buffer);

  static MouseButtonEvent::MouseButton get_mouse_button(sf::Mouse::Button);
  static Event* translateMouseEvent(sf::Event::MouseButtonEvent sf_mouse_data,
                                    MouseButtonEvent::Action action);
  static Event* translateKeyboardEvent(sf::Event::KeyEvent sf_key_data);

  Renderer();

 public:
  static void init(Size window_size, const char* name);
  static void deinit();

  static void init_offscreen_target(Size target_size, Position target_pos);
  static void flush_offscreen_target();

  static void clear();
  static void show();

  static Event* poll_event();

  static Image load_image(const char* filename);
  static void save_image(Image& img, const char* filename);

  static void draw_image(Position pos, Image& img);
  static void draw_rectangle(Size size, Position pos, Color color);
  static void draw_text(Text text, Position pos);
  static void draw_ellipse(Size size, Position pos, Color color);

  static void draw_sprite(Texture texture, Position pos);

  static void add_delayed(DelayedRenderData delayed_data);
  static void draw_delayed();
  static void remove_delayed();

  static void add_offset(Position offset);
  static Position get_offset(); 
  static void remove_offset();
};
#endif
