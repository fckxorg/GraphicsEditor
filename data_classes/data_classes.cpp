#include "data_classes.hpp"

/*---------------- SIZE CLASS -------------------------------*/

Size::Size() = default;
Size::Size(uint16_t width, uint16_t height) : width(width), height(height) {}
Size::Size(const sf::Vector2f& sfsize)
    : width(static_cast<uint16_t>(sfsize.x)),
      height(static_cast<uint16_t>(sfsize.y)) {}

Size::operator sf::Vector2f() const { return sf::Vector2f(width, height); }

/*---------------- COLOR CLASS --------------------------------*/

Color::Color() = default;
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a){};


Color::operator sf::Color() const { return sf::Color(r, g, b, a); }


/*--------------- POSITION ----------------------------------*/

Position::Position() = default;
Position::Position(uint16_t x, uint16_t y) : x(x), y(y) {}
Position::Position(const sf::Vector2f& sfpos)
        : x(static_cast<uint16_t>(sfpos.x)),
          y(static_cast<uint16_t>(sfpos.y)) {}

Position::operator sf::Vector2f() const { return sf::Vector2f(x, y); }
