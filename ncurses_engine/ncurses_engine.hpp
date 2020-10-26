#ifndef NCURSES_ENGINE_HPP
#define NCURSES_ENGINE_HPP

#include <ncurses.h>
#include <cstring>
#include <vector>
#include <cmath>

#include "../data_classes/data_classes.hpp"

class Renderer {
    private:
        int scale_x;
        int scale_y;

        int convert_to_ncurses_color(Color color);
        
        int get_pair(int fg_color, int bg_color);
    
        int rescale_value(const int value, const int scale);

    public:

        Renderer(Size window_size);
        ~Renderer();

        void draw_rectangle(Size size, Position pos, Color color);
            
        void draw_text(Text text, Position pos, Color bgcolor);

        void show();
};

#endif
