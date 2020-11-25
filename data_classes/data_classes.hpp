#ifndef DATA_CLASSES_HPP
#define DATA_CLASSES_HPP

#include <algorithm>
#include <cstdint>
#include <vector>

#ifdef SFML_ENGINE
#include <SFML/Graphics.hpp>
#endif

// conditional compilation or dedicated conversion fuctions or nothing?

struct Size {
  uint16_t width;
  uint16_t height;

  Size();

  Size(uint16_t width, uint16_t height);

// private and render engine as friend
#ifdef SFML_ENGINE
  explicit Size(const sf::Vector2f& sfsize);

  operator sf::Vector2f() const;
#endif
};

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  Color();

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

  Color operator+(int delta);
  Color operator-(int delta);

#ifdef SFML_ENGINE
  explicit Color(const sf::Color& sfcolor);

  operator sf::Color() const;
#endif
};

struct Position {
  int16_t x;
  int16_t y;

  Position();
  Position(int16_t x, int16_t y);

#ifdef SFML_ENGINE
  explicit Position(const sf::Vector2f& sfpos);

  operator sf::Vector2f() const;
#endif
};

struct Text {
  const char* text;
  uint16_t character_size;
  float line_spacing;
  const char* font_path;

  Color color;
  Color bg_color;

  Text();
  Text(const char* text, uint16_t character_size, const char* font_path,
       Color color, Color bg_color, float line_spacing = 1);
};

struct Viewport {
  Size size;
  Position pos;

  Viewport();
  Viewport(Size size, Position pos);
};

class Image {
 private:
  std::vector<uint8_t> pixels;
  Size size;

 public:
  Image(Size size, Color color);
  void setPixel(int x, int y, Color color);
  Color getPixel(int x, int y);
  const uint8_t* get_pixel_array();

  Size get_size();
};

struct Texture {
  const char* path;
  Size size;

  Texture();
  Texture(const char* path, Size size);
};

#endif
