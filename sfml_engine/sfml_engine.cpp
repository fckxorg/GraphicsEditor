#include "sfml_engine.hpp"

sf::RenderWindow Renderer::window;
std::unordered_map<const char*, sf::Font> Renderer::fonts; //TODO resource manager

void Renderer::init(Size window_size, const char* name) {
  assert(name != nullptr);

  window.create(sf::VideoMode(window_size.width, window_size.height), name);
  Renderer::clear();
}

void Renderer::deinit() { window.close(); }

void Renderer::clear() { window.clear(); }

void Renderer::show() { window.display(); }

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
  sf::RectangleShape rect = sf::RectangleShape(size);
  rect.setPosition(pos);
  rect.setFillColor(color);
  window.draw(rect);
}

void Renderer::draw_text(Text text, Position pos) {
  sf::Text sfml_text = Renderer::get_sfml_text(text);
  sfml_text.setPosition(pos);
  window.draw(sfml_text);
}

void Renderer::draw_scrollable_text(Text text, Size size, Position pos,
                                    Color color, float relative_offset) {
  sf::Text sfml_text = Renderer::get_sfml_text(text);
  sfml_text.setPosition(0, relative_offset);

  sf::RenderTexture viewport_texture;
  viewport_texture.create(size.width, size.height);
  viewport_texture.clear(sf::Color::White);

  viewport_texture.draw(sfml_text);
  viewport_texture.display();

  sf::Sprite viewport_sprite;
  viewport_sprite.setTexture(viewport_texture.getTexture());
  viewport_sprite.setPosition(pos);
  window.draw(viewport_sprite);
}

MouseButtonEvent::MouseButton Renderer::get_mouse_button(
    sf::Mouse::Button
        button) {  // XXX normal way to create private static const map?
  switch (button) {
    case sf::Mouse::Left:
      return MouseButtonEvent::MouseButton::LEFT;
    case sf::Mouse::Right:
      return MouseButtonEvent::MouseButton::RIGHT;
    case sf::Mouse::Middle:
      return MouseButtonEvent::MouseButton::MIDDLE;
    default:
      return MouseButtonEvent::MouseButton::UNDEFINED_BUTTON;
  }
}

Event* Renderer::translateMouseEvent(sf::Event::MouseButtonEvent sf_mouse_data,
                                     MouseButtonEvent::Action action) {
  Position pos(sf_mouse_data.x, sf_mouse_data.y);
  MouseButtonEvent::MouseButton button =
      Renderer::get_mouse_button(sf_mouse_data.button);

  MouseButtonEvent* event = new MouseButtonEvent(pos, button, action);

  return event;
}

Event* Renderer::poll_event() {
  sf::Event sf_event;

  if (!window.pollEvent(sf_event)) return nullptr;

  switch (sf_event.type) {
    case sf::Event::Closed: {
      return new WindowClosedEvent();
    }
    case sf::Event::MouseButtonPressed: {
      return Renderer::translateMouseEvent(sf_event.mouseButton,
                                           MouseButtonEvent::Action::PRESSED);
    }

    case sf::Event::MouseButtonReleased: {
      return Renderer::translateMouseEvent(sf_event.mouseButton,
                                           MouseButtonEvent::Action::RELEASED);
    }

    case sf::Event::MouseMoved: {
      Position pos(sf_event.mouseMove.x, sf_event.mouseMove.y);
      return new MouseMoveEvent(pos);
    }
  }

  return nullptr;
}

sf::Text Renderer::get_sfml_text(Text text) {
  if (!fonts.contains(text.font_path)) {
    sf::Font new_font;
    new_font.loadFromFile(text.font_path);
    fonts[text.font_path] = new_font;
  }

  sf::Text sfml_text(text.text, fonts[text.font_path], text.character_size);
  sfml_text.setLineSpacing(text.line_spacing);
  sfml_text.setFillColor(text.color);

  return sfml_text;
}

