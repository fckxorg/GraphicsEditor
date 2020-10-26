#include "sfml_engine.hpp"
#include <SFML/Graphics/Font.hpp>

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

void Renderer::draw_text(Text text, Position pos, Color bg_color) {
    fonts.push_back(sf::Font());  
    fonts.back().loadFromFile(text.get_font());

    sf::Text sfml_text(text.get_text(), fonts.back(), text.get_character_size());
    sfml_text.setPosition(pos);
    sfml_text.setFillColor(text.get_color());
    
    window.draw(sfml_text);
}

void Renderer::show() { window.display(); }
