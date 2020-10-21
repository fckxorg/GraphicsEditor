#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstdint>
#include <list>
#include <memory>

#include "../event/event.hpp"
#include "../render_data/render_data.hpp"
#include "../sfml_engine/sfml_engine.hpp"

// TODO adding window types via preprocessing
class Window {
   protected:
    uint32_t event_mask;
    bool opened;

   public:
    std::list<std::unique_ptr<Window>> subwindows;

    Window() = default;
    Window(uint32_t event_mask);
    virtual ~Window(){};

    virtual void open() = 0;
    virtual void close() = 0;

    bool is_opened() {
        return opened;
    }

    void add_child_window(std::unique_ptr<Window> child);

    void handle_event(Event event);
};

class RenderWindow : public Window {
   protected:
    Position pos;
    Size size;
    float rotation;

   public:
    RenderWindow() = default;

    virtual ~RenderWindow(){};

    RenderWindow(const RenderWindow& other) = delete;

    RenderWindow(Size size, Position pos) : size(size), pos(pos) {}

    void open() { opened = true; }

    void close() { opened = false; }

    void set_pos(Position pos) { this->pos = pos; }

    virtual Position get_position() const { return pos; }

    void set_rotation(const float rotation) { this->rotation = rotation; } 

    float get_rotation() const { return rotation; }

    virtual void render(Renderer& target) = 0;
};

class RectWindow : public RenderWindow {
   protected:
       Color color;
   public:
    RectWindow() = default;
    ~RectWindow() = default;
    RectWindow(Size size, Position pos, Color color) : RenderWindow(size, pos), color(color) {}

    void open() override {
        RenderWindow::open();
    }

    void close() override {
        RenderWindow::close();
    }

    virtual void render(Renderer& target) override {
        if (this->opened) {
            target.draw_rectangle(size, pos, color);
        }
    }
};

#endif
