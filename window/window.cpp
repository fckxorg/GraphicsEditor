#include "window.hpp"

/*--------------- WINDOW ---------------------*/

Window::Window() = default;
bool Window::is_opened() const {
    return opened;
}

void Window::set_event_mask(uint32_t mask) {
    event_mask = mask;
}

void Window::add_child_window(std::unique_ptr<Window>& child) {
    subwindows.push_back(std::move(child));
}

Window::~Window() = default;


/*----------- RENDER WINDOW ----------------*/

RenderWindow::RenderWindow() = default;
RenderWindow::~RenderWindow() = default;
RenderWindow::RenderWindow(Size size, Position pos) : size(size), pos(pos) {}

void RenderWindow::open() { opened = true; }
void RenderWindow::close() { opened = false; }

void RenderWindow::refresh() {
    
}

void RenderWindow::set_pos(Position pos) { this->pos = pos; }
Position RenderWindow::get_position() const { return pos; }

void RenderWindow::set_rotation(const float rotation) { this->rotation = rotation; } 
float RenderWindow::get_rotation() const { return rotation; }


/*---------- RECT WINDOW -------------------*/

RectWindow::RectWindow() = default;
RectWindow::~RectWindow() = default;
RectWindow::RectWindow(Size size, Position pos, Color color) : RenderWindow(size, pos), color(color) {}

void RectWindow::render() {
    if (!this->opened) {
        return;
    }
    
    Renderer::draw_rectangle(size, pos, color);

    for(auto& subwindow : subwindows) {
        subwindow->render();
    }
    
}

/* --------- RECT BUTTON -------------------*/
RectButton::RectButton() = default;
RectButton::~RectButton() = default;

void RectButton::render() {
    if(!this->opened) {
        return;
    }

    RectWindow::render();
    
    for(auto& subwindow : subwindows) {
        subwindow->render();
    }

}

/*--------------- TEXT WINDOW-----------------------------*/

TextWindow::TextWindow() = default;
TextWindow::TextWindow(Text text, Position pos, Color bgcolor) : text(text), bgcolor(bgcolor) {
    set_pos(pos);
}

Text TextWindow::get_text() const {
    return text;
}

void TextWindow::set_text(Text text) {
    this->text = text;
}

Color TextWindow::get_bgcolor() const {
    return bgcolor;
}

void TextWindow::set_bgcolor(Color bgcolor) {
    this->bgcolor = bgcolor;
}

void TextWindow::render() {
    Renderer::draw_text(text, pos, bgcolor);

    for(auto& subwindow : subwindows) {
        subwindow->render();
    }

}

