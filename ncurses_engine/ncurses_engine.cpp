#include "ncurses_engine.hpp"
#include <curses.h>

Renderer::Renderer(Size window_size) {
    initscr();
    resizeterm(window_size.height, window_size.width);
    start_color();
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    WINDOW* rect = newwin(size.height, size.width, pos.y, pos.x);

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
