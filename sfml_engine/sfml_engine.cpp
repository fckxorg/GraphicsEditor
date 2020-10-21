#include "sfml_engine.hpp"

Renderer::Renderer(Size window_size)
    : window(sf::VideoMode(window_size.width, window_size.height),
             "Test window") {
    window.clear(sf::Color::White);
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    sf::RectangleShape rect = sf::RectangleShape(size);
    rect.setPosition(pos);
    rect.setFillColor(color);
    window.draw(rect);
}

void Renderer::show() { window.display(); }
