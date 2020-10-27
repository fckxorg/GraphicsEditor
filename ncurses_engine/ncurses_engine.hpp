#ifndef NCURSES_ENGINE_HPP
#define NCURSES_ENGINE_HPP

#include <ncurses.h>
#include <cstring>
#include <vector>
#include <cmath>

#include "../data_classes/data_classes.hpp"

class Renderer {
    private:
        static int scale_x;
        static int scale_y;

        static int convert_to_ncurses_color(Color color);
        
        static int get_pair(int fg_color, int bg_color);
    
        static int rescale_value(const int value, const int scale);
        
        Renderer(); 

    public:

        static void init(Size window_size);
        static void deinit();

        static void draw_rectangle(Size size, Position pos, Color color);
            
        static void draw_text(Text text, Position pos, Color bgcolor);

        static void show();
};

#endif
