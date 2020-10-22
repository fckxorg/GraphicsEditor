#ifndef DATA_CLASSES_HPP
#define DATA_CLASSES_HPP


#include <bits/stdint-uintn.h>
#include <cstdint>

#ifdef SFML_ENGINE
#include <SFML/Graphics.hpp>
#endif

// conditional compilation or dedicated conversion fuctions or nothing?

struct Size {
    uint16_t width;
    uint16_t height;

    Size();

    Size(uint16_t width, uint16_t height);

// private and render engine as friend    
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

struct Text {
    const char* text;
    uint16_t character_size;
    const char* font_path;
    Color color;
    
    Text();
    Text(const char* text, uint16_t character_size, const char* font_path, Color color);

    void set_text(const char* text);
    const char* get_text() const;
    
    void set_font(const char* font);
    const char* get_font() const;


    void set_character_size(uint16_t character_size);
    uint16_t get_character_size();

    void set_color(Color color);
    Color get_color();
};

#endif
