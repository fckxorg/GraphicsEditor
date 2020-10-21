#ifndef DATA_CLASSES_HPP
#define DATA_CLASSES_HPP

#include <cstdint>
#include <SFML/Graphics.hpp>

struct Size {
    uint16_t width;
    uint16_t height;

    Size();

    Size(uint16_t width, uint16_t height);

    explicit Size(const sf::Vector2f& sfsize);

    operator sf::Vector2f() const;
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color();

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    explicit Color(const sf::Color& sfcolor);

    operator sf::Color() const;
};

struct Position {
    uint16_t x;
    uint16_t y;

    Position();
    Position(uint16_t x, uint16_t y);

    explicit Position(const sf::Vector2f& sfpos);

    operator sf::Vector2f() const;
};

#endif
