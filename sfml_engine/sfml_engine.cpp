#include "sfml_engine.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <stdio.h>

#include <SFML/Graphics/RenderTexture.hpp>
#include <unordered_map>

sf::RenderWindow Renderer::window;
std::unordered_map<const char*, sf::Font> Renderer::fonts;

void Renderer::clear() { window.clear(); }

void Renderer::init(Size window_size, const char* name) {
    window.create(sf::VideoMode(window_size.width, window_size.height), name);
    Renderer::clear();
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    sf::RectangleShape rect = sf::RectangleShape(size);
    rect.setPosition(pos);
    rect.setFillColor(color);
    window.draw(rect);
}

void Renderer::draw_text(Text text, Position pos, Color bg_color) {
    if (!fonts.contains(text.get_font())) {
        sf::Font new_font;
        new_font.loadFromFile(text.get_font());
        fonts[text.get_font()] = new_font;
    }

    sf::Text sfml_text(text.get_text(), fonts[text.get_font()],
                       text.get_character_size());
    sfml_text.setPosition(pos);
    sfml_text.setLineSpacing(text.line_spacing);
    sfml_text.setFillColor(text.get_color());

    window.draw(sfml_text);
}

void Renderer::draw_scrollable_text(Text text, Size size, Position pos,
                                    Color color, int16_t relative_offset) {
    if (!fonts.contains(text.get_font())) {
        sf::Font new_font;
        new_font.loadFromFile(text.get_font());
        fonts[text.get_font()] = new_font;
    }

    sf::Text sfml_text(text.get_text(), fonts[text.get_font()],
                       text.get_character_size());
    sfml_text.setPosition(0, relative_offset);
    sfml_text.setFillColor(text.get_color());
    sfml_text.setLineSpacing(text.line_spacing);

    sf::RenderTexture viewport_texture;
    viewport_texture.create(size.width, size.height);
    viewport_texture.clear(sf::Color::White);
    
    viewport_texture.draw(sfml_text);
    viewport_texture.display();


    sf::Sprite viewport_sprite;
    viewport_sprite.setTexture(viewport_texture.getTexture());
    viewport_sprite.setPosition(pos);
    window.draw(viewport_sprite);

}

void Renderer::deinit() { window.close(); }

void Renderer::show() { window.display(); }

MouseButtonEvent::MouseButton Renderer::get_mouse_button(
    sf::Mouse::Button button) {
    switch (button) {
        case sf::Mouse::Left:
            return MouseButtonEvent::MouseButton::LEFT;
        case sf::Mouse::Right:
            return MouseButtonEvent::MouseButton::RIGHT;
        case sf::Mouse::Middle:
            return MouseButtonEvent::MouseButton::MIDDLE;
        default:
            return MouseButtonEvent::MouseButton::UNDEFINED_BUTTON;
    }
}

Event* Renderer::translateMouseEvent(sf::Event::MouseButtonEvent sf_mouse_data,
                                     MouseButtonEvent::Action action) {
    Position pos(sf_mouse_data.x, sf_mouse_data.y);
    MouseButtonEvent::MouseButton button =
        Renderer::get_mouse_button(sf_mouse_data.button);

    MouseButtonEvent* event = new MouseButtonEvent(pos, button, action);

    return event;
}

Event* Renderer::poll_event() {
    sf::Event sf_event;

    if (!window.pollEvent(sf_event)) return nullptr;

    switch (sf_event.type) {
        case sf::Event::Closed: {
            return new WindowClosedEvent();
        }
        case sf::Event::MouseButtonPressed: {
            return Renderer::translateMouseEvent(
                sf_event.mouseButton, MouseButtonEvent::Action::PRESSED);
        }

        case sf::Event::MouseButtonReleased: {
            return Renderer::translateMouseEvent(
                sf_event.mouseButton, MouseButtonEvent::Action::RELEASED);
        }

        case sf::Event::MouseMoved: {
            Position pos(sf_event.mouseMove.x, sf_event.mouseMove.y);
            return new MouseMoveEvent(pos);
        }
    }

    return nullptr;
}

