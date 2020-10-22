#include "ncurses_engine.hpp"
#include <curses.h>

Renderer::Renderer(Size window_size) {
    initscr();
    refresh();
    start_color();
    scale_x = window_size.width / COLS; 
    scale_y = window_size.height / LINES;
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    int win_size_x = size.width / scale_x;
    int win_size_y = size.height / scale_y;
    int win_pos_x = pos.x / (2 * scale_x);
    int win_pos_y = pos.y / scale_y;

    WINDOW* rect = newwin(win_size_y, win_size_x, win_pos_y, win_size_x);

    init_color(16, color.r, color.b, color.g);
    init_pair(1, COLOR_BLUE, 16); // how to add more colors?
    wbkgd(rect, ' ' | COLOR_PAIR(1));
    touchwin(rect);
    refresh();
    wrefresh(rect);
}

void Renderer::show() {
    refresh();
    getch();
}
