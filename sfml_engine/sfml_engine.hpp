#ifndef SFMLEngine
#define SFMLEngine


#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "../data_classes/data_classes.hpp"

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
