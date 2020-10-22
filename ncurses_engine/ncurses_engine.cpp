#include "ncurses_engine.hpp"
#include <curses.h>

Renderer::Renderer(Size window_size) {
    initscr();
    refresh();
    start_color();
    scale_x = window_size.width / COLS; 
    scale_y = window_size.height / LINES;

    colors.assign(16, 0);
}

Renderer::~Renderer() {
    endwin();
}

int Renderer::rescale_value(const int value, const int scale) {
    return value / scale;
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    int win_size_x = size.width / scale_x;
    int win_size_y = size.height / scale_y;
    int win_pos_x = pos.x / (2 * scale_x);
    int win_pos_y = pos.y / scale_y;

    // TODO add vector with colors
    WINDOW* rect = newwin(win_size_y, win_size_x, win_pos_y, win_pos_x);

    init_color(colors.size(), color.r, color.g, color.b);
    colors.push_back(colors.size());

    init_pair(color_pairs.size() + 1, COLOR_BLACK, colors.size() - 1);
    color_pairs.push_back(color_pairs.size() + 1);
    wbkgd(rect, ' ' | COLOR_PAIR(color_pairs.back()));

    touchwin(rect);
    wrefresh(rect);
    refresh();
}

void Renderer::draw_text(Text text, Position pos, Color bgcolor) {
    init_color(colors.size(), text.color.r, text.color.g, text.color.b);  
    colors.push_back(colors.size());

    init_color(colors.size(), bgcolor.r, bgcolor.g, bgcolor.b);
    colors.push_back(colors.size());

    init_pair(color_pairs.size() + 1, colors.size() - 2, colors.size() - 1);
    color_pairs.push_back(color_pairs.size() + 1);

    
    int win_pos_x = rescale_value(pos.x, 2 * scale_x);
    int win_pos_y = rescale_value(pos.y, scale_y);
    int win_size_x = strlen(text.get_text());


    WINDOW* text_window = newwin(1, win_size_x, win_pos_y, win_pos_x);
    wattron(text_window, COLOR_PAIR(color_pairs.back()));
    mvwprintw(text_window, 0, 0, text.get_text());
    touchwin(text_window);
    wrefresh(text_window);
    refresh();
}

void Renderer::show() {
    refresh();
    getch();
}
