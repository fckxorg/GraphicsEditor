#ifndef DATA_CLASSES_HPP
#define DATA_CLASSES_HPP


#include <cstdint>

#ifdef SFML_ENGINE
#include <SFML/Graphics.hpp>
#endif

// conditional compilation or dedicated conversion fuctions or nothing

struct Size {
    uint16_t width;
    uint16_t height;

    Size();

    Size(uint16_t width, uint16_t height);

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

#ifdef SFML_ENGINE
    explicit Color(const sf::Color& sfcolor);

    operator sf::Color() const;
#endif
};

struct Position {
    uint16_t x;
    uint16_t y;

    Position();
    Position(uint16_t x, uint16_t y);

#ifdef SFML_ENGINE
    explicit Position(const sf::Vector2f& sfpos);

    operator sf::Vector2f() const;
#endif
};

#endif
