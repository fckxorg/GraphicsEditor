#ifndef SFMLEngine
#define SFMLEngine

#include <bits/stdint-uintn.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

struct Size {
    uint16_t width;
    uint16_t height;

    Size() = default;

    Size(uint16_t width, uint16_t height) : width(width), height(height) {}

    explicit Size(const sf::Vector2f& sfsize)
        : width(static_cast<uint16_t>(sfsize.x)),
          height(static_cast<uint16_t>(sfsize.y)) {}

    operator sf::Vector2f() const { return sf::Vector2f(width, height); }
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    Color() = default;

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a){};

    explicit Color(const sf::Color& sfcolor)
        : r(sfcolor.r), g(sfcolor.g), b(sfcolor.b), a(sfcolor.a) {}

    operator sf::Color() const { return sf::Color(r, g, b, a); }
};

struct Position {
    uint16_t x;
    uint16_t y;

    Position() = default;
    Position(uint16_t x, uint16_t y) : x(x), y(y) {}

    explicit Position(const sf::Vector2f& sfpos)
        : x(static_cast<uint16_t>(sfpos.x)),
          y(static_cast<uint16_t>(sfpos.y)) {}

    operator sf::Vector2f() const { return sf::Vector2f(x, y); }
};

class Renderer {
   private:
    sf::RenderWindow window;

   public:
    Renderer(Size window_size)
        : window(sf::VideoMode(window_size.width, window_size.height),
                 "Test window") {
        window.clear(sf::Color::White);
    }

    void draw_rectangle(Size size, Position pos, Color color) {
        sf::RectangleShape rect = sf::RectangleShape(size);
        rect.setPosition(pos);
        rect.setFillColor(color);
        window.draw(rect);
    }

    void show() { window.display(); }
};

#endif
