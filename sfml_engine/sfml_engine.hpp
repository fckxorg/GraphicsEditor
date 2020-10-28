#ifndef SFML_ENGINE_HPP
#define SFML_ENGINE_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"

class Renderer {
   private:
    static sf::RenderWindow window;

    static std::vector<sf::Font> fonts;

    static MouseButton get_mouse_button(sf::Mouse::Button);
    static MouseButtonEvent translateMouseEvent(sf::Event::MouseButtonEvent sf_mouse_data);

    Renderer();

   public:
    static void clear();
    static void draw_rectangle(Size size, Position pos, Color color); 
    static void draw_text(Text text, Position pos, Color bg_color);
    static void init(Size window_size, const char* name);
    static void show();
    static void deinit();

    static bool poll_event(Event& event);
};
#endif
