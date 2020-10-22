#include "ncurses_engine.hpp"

Renderer::Renderer(Size window_size) {
    initscr();
}

Renderer::~Renderer() {
    endwin();
}

void Renderer::draw_rectangle(Size size, Position pos, Color color) {
    printw("I am rectangle!");
}

void Renderer::show() {
    refresh();
    getch();
}
