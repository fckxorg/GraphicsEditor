#include "ncurses_engine.hpp"

#include <curses.h>

#include <cstdio>

int Renderer::scale_x = 0;
int Renderer::scale_y = 0;

void Renderer::init(Size window_size, const char* name) {
    initscr();
    refresh();
    start_color();

    Renderer::scale_x = window_size.width / COLS;
    Renderer::scale_y = window_size.height / LINES;
}

void Renderer::clear() { erase(); }

void Renderer::deinit() { endwin(); }

int Renderer::convert_to_ncurses_color(Color color) {
    short r = 0;
    short g = 0;
    short b = 0;

    short color_r = color.r * 1000 / 255;
    short color_g = color.g * 1000 / 255;
    short color_b = color.b * 1000 / 255;

    float diff = MAXFLOAT;
    int idx = 0;

    for (int i = 0; i < COLORS; ++i) {
        color_content(i, &r, &g, &b);

        short diff_r = r - color_r;
        short diff_g = g - color_g;
        short diff_b = b - color_b;

        float cur_diff =
            sqrt(diff_r * diff_r + diff_g * diff_g + diff_b * diff_b);

        if (cur_diff < diff) {
            idx = i;
            diff = cur_diff;
        }
    }

    return idx;
}

int Renderer::get_pair(int fg_color, int bg_color) {
    int empty_pair_idx = -1;
    short fg_pair_color = 0;
    short bg_pair_color = 0;

    for (short i = 1; i < COLOR_PAIRS; ++i) {
        pair_content(i, &fg_pair_color, &bg_pair_color);

        if (fg_color == fg_pair_color && bg_color == bg_pair_color) {
            return i;
        }

        if (fg_pair_color == 0 && bg_pair_color == 0) {
            empty_pair_idx = i;
            break;
        }
    }

    init_pair(empty_pair_idx, fg_color, bg_color);

    return empty_pair_idx;
}

int Renderer::rescale_value(const int value, const int scale) {
    return value / scale;
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    int win_size_x = size.width / scale_x;
    int win_size_y = size.height / scale_y;
    int win_pos_x = pos.x / (2 * scale_x);
    int win_pos_y = pos.y / scale_y;

    WINDOW* rect = newwin(win_size_y, win_size_x, win_pos_y, win_pos_x);

    int ncurses_color = convert_to_ncurses_color(color);
    int pair = get_pair(COLOR_BLACK, ncurses_color);
    wbkgd(rect, ' ' | COLOR_PAIR(pair));

    touchwin(rect);
    wrefresh(rect);
    refresh();
}

void Renderer::draw_text(Text text, Position pos, Color bgcolor) {
    int fg_color = convert_to_ncurses_color(text.color);
    int bg_color = convert_to_ncurses_color(bgcolor);

    int pair = get_pair(fg_color, bg_color);

    int win_pos_x = rescale_value(pos.x, 2 * scale_x);
    int win_pos_y = rescale_value(pos.y, scale_y);
    int win_size_x = strlen(text.get_text());

    WINDOW* text_window = newwin(1, win_size_x, win_pos_y, win_pos_x);
    wattron(text_window, COLOR_PAIR(pair));
    mvwprintw(text_window, 0, 0, text.get_text());
    touchwin(text_window);
    wrefresh(text_window);
    refresh();
}

void Renderer::show() {
    refresh();
    getch();
}

