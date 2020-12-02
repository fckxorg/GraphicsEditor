#include "sfml_engine.hpp"

#include <SFML/Window/Keyboard.hpp>

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
std::unordered_map<const char*, sf::Texture> Renderer::textures;
std::stack<Position> Renderer::global_offsets;
bool Renderer::has_delayed = false;

DelayedRenderData Renderer::delayed_render = {};

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
  target.sprite->setPosition((target_pos += get_offset()));
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

  while (!global_offsets.empty()) {
    global_offsets.pop();
  }

  global_offsets.push(Position(0, 0));
}

sf::RenderTarget* Renderer::get_target() {
  if (offscreen_render_stack.empty()) {
    return &window;
  }

  return offscreen_render_stack.top().texture;
}

Position Renderer::get_offset() { return global_offsets.top(); }
void Renderer::add_offset(Position offset) { global_offsets.push(offset); }

void Renderer::show() { window.display(); }

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
  sf::RectangleShape rect = sf::RectangleShape(size);
  rect.setPosition((pos += get_offset()));
  rect.setFillColor(color);

  auto target = get_target();
  target->draw(rect);
}

void Renderer::draw_text(Text text, Position pos) {
  sf::Text sfml_text = Renderer::get_sfml_text(text);
  sfml_text.setPosition((pos += get_offset()));

  auto target = get_target();
  target->draw(sfml_text);
}

MouseButtonEvent::MouseButton Renderer::get_mouse_button(
    sf::Mouse::Button button) {
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

Event* Renderer::translateKeyboardEvent(sf::Event::KeyEvent sf_key_data) {
  KEY key = UNDEFINED;
  if (sf_key_data.code < 26) {
    key = static_cast<KEY>(sf_key_data.code);
  }
  switch (sf_key_data.code) {
    case sf::Keyboard::Enter: {
      key = KEY::Return;
      break;
    }
    case sf::Keyboard::Backspace: {
      key = KEY::Backspace;
      break;
    }
    case sf::Keyboard::Left: {
      key = KEY::Left;
      break;
    }

    case sf::Keyboard::Right: {
      key = KEY::Right;
      break;
    }
    case sf::Keyboard::Space: {
      key = KEY::Space;
      break;
    }
  }
  return new KeyPressedEvent(key, sf_key_data.shift, sf_key_data.control);
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

    case sf::Event::KeyPressed: {
      return translateKeyboardEvent(sf_event.key);
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

Image Renderer::load_image(const char* filename) {
  sf::Image sf_img;
  sf_img.loadFromFile(filename);

  sf::Vector2u img_size = sf_img.getSize();

  Image img(Size(img_size.x, img_size.y), Color(255, 255, 255));

  for (int x = 0; x < img_size.x; ++x) {
    for (int y = 0; y < img_size.y; ++y) {
      auto sfml_color = sf_img.getPixel(x, y);
      img.setPixel(
          x, y, Color(sfml_color.r, sfml_color.g, sfml_color.b, sfml_color.a));
    }
  }

  return img;
}

void Renderer::save_image(Image& img, const char* filename) {
  sf::Image sf_img;
  sf_img.create(img.get_size().width, img.get_size().height, sf::Color::Black);

  for (int x = 0; x < img.get_size().width; ++x) {
    for (int y = 0; y < img.get_size().height; ++y) {
      sf_img.setPixel(x, y, img.getPixel(x, y));
    }
  }

  sf_img.saveToFile(filename);
}

void Renderer::draw_image(Position pos, Image& img) {
  sf::Texture img_texture;
  img_texture.create(img.get_size().width, img.get_size().height);
  img_texture.update(img.get_pixel_array());

  sf::Sprite img_sprite(img_texture);

  img_sprite.setPosition((pos += get_offset()));

  auto target = get_target();
  target->draw(img_sprite);
}

void Renderer::draw_sprite(Texture texture, Position pos) {
  if (!textures.contains(texture.path)) {
    sf::Texture new_texture;
    new_texture.loadFromFile(texture.path);
    textures.insert({texture.path, new_texture});
  }

  sf::Sprite sfml_sprite(textures[texture.path]);
  sfml_sprite.setPosition((pos += get_offset()));
  auto target = get_target();
  target->draw(sfml_sprite);
}

void Renderer::draw_delayed() {
  if (has_delayed) {
    switch (delayed_render.type) {
      case RECT: {
        Renderer::draw_rectangle(delayed_render.size, delayed_render.pos += get_offset(),
                                 delayed_render.color);
        break;
      }

      case ELLIPSE: {
        Renderer::draw_ellipse(delayed_render.size, delayed_render.pos += get_offset(),
                               delayed_render.color);
        break;
      }
    }
  }
}

void Renderer::add_delayed(DelayedRenderData delayed_data) {
  has_delayed = true;
  Renderer::delayed_render = delayed_data;
}

void Renderer::remove_delayed() { has_delayed = false; }

void Renderer::draw_ellipse(Size size, Position pos, Color color) {
  sf::CircleShape ellipse(std::max(abs(size.width), abs(size.height)) / 2);

  if (size.width < 0) {
    pos.x += size.width;
  }

  if (size.height < 0) {
    pos.y += size.height;
  }

  ellipse.setFillColor(color);
  ellipse.setPosition((pos += get_offset()));

  float scale = 0;

  if (abs(size.width) > abs(size.height)) {
    scale = fabs(static_cast<float>(size.height) / size.width);
    ellipse.setScale(1.f, scale);
  } else {
    scale = fabs(static_cast<float>(size.width) / size.height);
    ellipse.setScale(scale, 1.f);
  }

  auto target = get_target();
  target->draw(ellipse);
}

void Renderer::remove_offset() {
    global_offsets.pop();
}
