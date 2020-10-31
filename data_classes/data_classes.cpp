#include "data_classes.hpp"

/*---------------- SIZE CLASS -------------------------------*/

Size::Size() = default;
Size::Size(uint16_t width, uint16_t height) : width(width), height(height) {}

#ifdef SFML_ENGINE
Size::Size(const sf::Vector2f& sfsize)
    : width(static_cast<uint16_t>(sfsize.x)),
      height(static_cast<uint16_t>(sfsize.y)) {}

Size::operator sf::Vector2f() const { return sf::Vector2f(width, height); }
#endif

/*---------------- COLOR CLASS --------------------------------*/

Color::Color() = default;
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a){};

#ifdef SFML_ENGINE
Color::operator sf::Color() const { return sf::Color(r, g, b, a); }
#endif

/*--------------- POSITION ----------------------------------*/

Position::Position() = default;
Position::Position(uint16_t x, uint16_t y) : x(x), y(y) {}

#ifdef SFML_ENGINE
Position::Position(const sf::Vector2f& sfpos)
        : x(static_cast<uint16_t>(sfpos.x)),
          y(static_cast<uint16_t>(sfpos.y)) {}

Position::operator sf::Vector2f() const { return sf::Vector2f(x, y); }
#endif

/*--------------- TEXT -------------------------------------*/

Text::Text() = default;
Text::Text(const char* text, uint16_t character_size, const char* font_path, Color color, float line_spacing) : text(text), character_size(character_size), font_path(font_path), color(color), line_spacing(line_spacing){}

void Text::set_text(const char* text) {
    this->text = text;
}

const char* Text::get_text() const {
    return text;
}
    
void Text::set_font(const char* font) {
    font_path = font;
}

const char* Text::get_font() const {
    return font_path;
}


void Text::set_character_size(uint16_t character_size) {
    this->character_size = character_size;
}

uint16_t Text::get_character_size() {
    return character_size;
}

Color Text::get_color() {
    return color;
}

void Text::set_color(Color color) {
    this->color = color;
}

