#include "sfml_engine.hpp"

sf::RenderWindow Renderer::window;
std::vector<sf::Font> Renderer::fonts;

void Renderer::clear() {
    window.clear(sf::Color::White);
}

void Renderer::init(Size window_size) {
    window.create(sf::VideoMode(window_size.width, window_size.height), "Test window");
    Renderer::clear();
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

void Renderer::deinit() {
    window.close();
}

void Renderer::show() { window.display(); }
