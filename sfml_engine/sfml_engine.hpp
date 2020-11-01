#ifndef SFML_ENGINE_HPP
#define SFML_ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <unordered_map>
#include <cassert>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"

class Renderer {
 private:
  static sf::RenderWindow window;

  static std::unordered_map<const char*, sf::Font> fonts;

  static MouseButtonEvent::MouseButton get_mouse_button(sf::Mouse::Button);
  static Event* translateMouseEvent(sf::Event::MouseButtonEvent sf_mouse_data,
                                    MouseButtonEvent::Action action);

  Renderer();

 public:
  static void init(Size window_size, const char* name);
  static void deinit();

  static void clear();
  static void show();

  static Event* poll_event();

  static void draw_rectangle(Size size, Position pos, Color color);
  static void draw_text(Text text, Position pos, Color bg_color);
  static void draw_scrollable_text(Text text, Size size, Position pos,
                                   Color color, float relative_offset);

};
#endif
