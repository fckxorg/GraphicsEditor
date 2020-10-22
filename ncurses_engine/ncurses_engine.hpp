#ifndef NCURSES_ENGINE_HPP
#define NCURSES_ENGINE_HPP

#include <ncurses.h>

#include "../data_classes/data_classes.hpp"

class Renderer {
    public:
        Renderer(Size window_size);
        ~Renderer();

        void draw_rectangle(Size size, Position pos, Color color);

        void show();
};

#endif
