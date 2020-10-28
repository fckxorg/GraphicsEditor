#include "sfml_engine.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

sf::RenderWindow Renderer::window;
std::vector<sf::Font> Renderer::fonts;

void Renderer::clear() {
    window.clear();
}

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
    fonts.push_back(sf::Font());
    fonts.back().loadFromFile(text.get_font());

    sf::Text sfml_text(text.get_text(), fonts.back(),
                       text.get_character_size());
    sfml_text.setPosition(pos);
    sfml_text.setFillColor(text.get_color());

    window.draw(sfml_text);
}

void Renderer::deinit() { window.close(); }

void Renderer::show() { window.display(); }

MouseButton Renderer::get_mouse_button(sf::Mouse::Button button) {
    switch (button) {
        case sf::Mouse::Left:
            return MouseButton::LEFT;
        case sf::Mouse::Right:
            return MouseButton::RIGHT;
        case sf::Mouse::Middle:
            return MouseButton::MIDDLE;
        default:
            return UNDEFINED_BUTTON;
    }
}

MouseButtonEvent Renderer::translateMouseEvent(
    sf::Event::MouseButtonEvent sf_mouse_data) {
    Position pos(sf_mouse_data.x, sf_mouse_data.y);
    MouseButton button = Renderer::get_mouse_button(sf_mouse_data.button);
    MouseButtonEvent mouse_event_data = {pos, button};

    return mouse_event_data;
}

bool Renderer::poll_event(Event& event) {
    sf::Event sf_event;

    if (!window.pollEvent(sf_event)) return false;

    switch (sf_event.type) {
        case sf::Event::Closed: {
            event.type = WINDOW_CLOSED;
            break;
        }
        case sf::Event::MouseButtonPressed: {
            event.type = MOUSE_BUTTON_PRESSED;
            event.mouse_button_event =
                Renderer::translateMouseEvent(sf_event.mouseButton);
            break;
        }

        case sf::Event::MouseButtonReleased: {
            event.type = MOUSE_BUTTON_RELEASED;
            event.mouse_button_event =
                Renderer::translateMouseEvent(sf_event.mouseButton);
            break;
        }

        case sf::Event::MouseMoved: {
            event.type = MOUSE_MOVED;
            Position pos(sf_event.mouseMove.x, sf_event.mouseMove.y);
            event.mouse_move_event = {pos};
            break;
        }

        default: {
            Event undefined_event = {UNDEFINED_EVENT};
            break;
        }
    }

    return true;
}

