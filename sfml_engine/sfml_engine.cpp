#include "sfml_engine.hpp"

OffscreenRenderData::OffscreenRenderData() {
  sprite = new sf::Sprite();
  texture = new sf::RenderTexture();
}

void OffscreenRenderData::release() {
  delete sprite;
  delete texture;
}

std::stack<OffscreenRenderData> Renderer::offscreen_render_stack;
std::vector<OffscreenRenderData> Renderer::offscreen_resources;

sf::RenderWindow Renderer::window;
std::unordered_map<const char*, sf::Font>
    Renderer::fonts;  // TODO resource manager

void Renderer::init(Size window_size, const char* name) {
  assert(name != nullptr);

  window.create(sf::VideoMode(window_size.width, window_size.height), name);
  Renderer::clear();
}

void Renderer::deinit() { window.close(); }

void Renderer::init_offscreen_target(Size target_size, Position target_pos) {
  OffscreenRenderData target = {};
  target.sprite->setPosition(target_pos);
  target.texture->create(target_size.width, target_size.height);
  target.sprite->setTexture(target.texture->getTexture());

  offscreen_resources.push_back(target);
  offscreen_render_stack.push(target);
}

void Renderer::flush_offscreen_target() {
  auto cur_texture = offscreen_render_stack.top();
  cur_texture.texture->display();
  offscreen_render_stack.pop();

  if (offscreen_render_stack.size() > 1) {
    offscreen_render_stack.top().texture->draw(*cur_texture.sprite);
    offscreen_render_stack.top().texture->display();
  } else {
    window.draw(*cur_texture.sprite);
  }
}

void Renderer::clear() {
  while (offscreen_render_stack.size()) {
    offscreen_render_stack.pop();
  }
  offscreen_resources.clear();
  window.clear();
}

sf::RenderTarget* Renderer::get_target() {
  if (offscreen_render_stack.empty()) {
    return &window;
  }

  return offscreen_render_stack.top().texture;
}

void Renderer::show() { window.display(); }

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
  sf::RectangleShape rect = sf::RectangleShape(size);
  rect.setPosition(pos);
  rect.setFillColor(color);

  auto target = get_target();
  target->draw(rect);
}

void Renderer::draw_text(Text text, Position pos) {
  sf::Text sfml_text = Renderer::get_sfml_text(text);
  sfml_text.setPosition(pos);

  auto target = get_target();
  target->draw(sfml_text);
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

std::vector<std::vector<Color>> Renderer::load_image(const char* filename) {
  sf::Image img;
  img.loadFromFile(filename);

  sf::Vector2u img_size = img.getSize();

  std::vector<std::vector<Color>> buffer(
      img_size.x, std::vector<Color>(img_size.y, Color()));

  for (int x = 0; x < img_size.x; ++x) {
    for (int y = 0; y < img_size.y; ++y) {
      buffer[x][y] = Color(img.getPixel(x, y));
    }
  }

  return buffer;
}

static void save_image(std::vector<Color> buffer,
                       const char* filename) {
  sf::Image img;
  img.create(buffer.size(), buffer[0].size(), sf::Color::Black);

  for (int x = 0; x < buffer.size(); ++x) {
    for (int y = 0; y < buffer[0].size(); ++y) {
      img.setPixel(x, y, buffer[x][y]);
    }
  }

  img.saveToFile(filename);
}
