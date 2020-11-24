#include "data_classes.hpp"

#include <bits/stdint-uintn.h>

/*---------------- SIZE CLASS -------------------------------*/

Size::Size() = default;
Size::Size(uint16_t width, uint16_t height) : width(width), height(height) {}

#ifdef SFML_ENGINE
Size::Size(const sf::Vector2f& sfsize)
    : width(static_cast<uint16_t>(sfsize.x)),
      height(static_cast<uint16_t>(sfsize.y)) {}

Size::operator sf::Vector2f() const { return sf::Vector2f(width, height); }
#endif

/*---------------- COLOR CLASS --------------------------------*/

Color::Color() = default;
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a){};

Color Color::operator+(int delta) {
  uint8_t color_r = std::max(static_cast<int>(r) + delta, 0);
  uint8_t color_g = std::max(static_cast<int>(g) + delta, 0);
  uint8_t color_b = std::max(static_cast<int>(b) + delta, 0);

  return Color(color_r, color_g, color_b, a);
}

Color Color::operator-(int delta) {
  delta = -delta;
  return *this + delta;
}

#ifdef SFML_ENGINE
Color::operator sf::Color() const { return sf::Color(r, g, b, a); }
#endif

/*--------------- POSITION ----------------------------------*/

Position::Position() = default;
Position::Position(int16_t x, int16_t y) : x(x), y(y) {}

#ifdef SFML_ENGINE
Position::Position(const sf::Vector2f& sfpos)
    : x(static_cast<uint16_t>(sfpos.x)), y(static_cast<uint16_t>(sfpos.y)) {}

Position::operator sf::Vector2f() const { return sf::Vector2f(x, y); }
#endif

/*--------------- TEXT -------------------------------------*/

Text::Text() = default;
Text::Text(const char* text, uint16_t character_size, const char* font_path,
           Color color, Color bg_color, float line_spacing)
    : text(text),
      character_size(character_size),
      font_path(font_path),
      color(color),
      bg_color(bg_color),

      line_spacing(line_spacing) {}

/*--------------- VIEWPORT ---------------------------------*/
Viewport::Viewport() = default;
Viewport::Viewport(Size size, Position pos) : size(size), pos(pos) {}

/*-------------------- IMAGE -----------------------------*/
Image::Image(Size size, Color color) : size(size) {
  pixels.assign(size.width * size.height * sizeof(Color), 0);
  for (int i = 0; i < pixels.size() * sizeof(Color); i += sizeof(Color)) {
    pixels[i] = color.r;
    pixels[i + 1] = color.g;
    pixels[i + 2] = color.b;
    pixels[i + 3] = color.a;
  }
}

void Image::setPixel(int x, int y, Color color) {
  int pos = (y * size.width + x) * sizeof(Color);
  pixels[pos] = color.r;
  pixels[pos + 1] = color.g;
  pixels[pos + 2] = color.b;
  pixels[pos + 3] = color.a;
}

Color Image::getPixel(int x, int y) {
  return *reinterpret_cast<Color*>(
      (pixels.data() + (y * size.width + x) * sizeof(Color)));
}

const uint8_t* Image::get_pixel_array() { return pixels.data(); }
