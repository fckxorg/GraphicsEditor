#include "window.hpp"
#include <bits/stdint-uintn.h>


/*--------------- WINDOW ---------------------*/

Window::Window() = default;
bool Window::is_opened() const {
    return opened;
}

void Window::set_event_mask(uint32_t mask) {
    event_mask = mask;
}

void Window::add_child_window(std::unique_ptr<Window> child) {
    //subwindows.push_back(child);
}

Window::~Window() = default;


/*----------- RENDER WINDOW ----------------*/

RenderWindow::RenderWindow() = default;
RenderWindow::~RenderWindow() = default;
RenderWindow::RenderWindow(Size size, Position pos) : size(size), pos(pos) {}

void RenderWindow::open() { opened = true; }
void RenderWindow::close() { opened = false; }

void RenderWindow::set_pos(Position pos) { this->pos = pos; }
Position RenderWindow::get_position() const { return pos; }

void RenderWindow::set_rotation(const float rotation) { this->rotation = rotation; } 
float RenderWindow::get_rotation() const { return rotation; }


/*---------- RECT WINDOW -------------------*/

RectWindow::RectWindow() = default;
RectWindow::~RectWindow() = default;
RectWindow::RectWindow(Size size, Position pos, Color color) : RenderWindow(size, pos), color(color) {}

void RectWindow::render(Renderer& target) {
    if (this->opened) {
        target.draw_rectangle(size, pos, color);
    }
}
