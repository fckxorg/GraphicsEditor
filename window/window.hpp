#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics/Texture.hpp>
#include <bits/stdint-uintn.h>
#include <cstdint>
#include <list>
#include <memory>

#include "../sfml_engine/sfml_engine.hpp"

class Window {
   protected:
    uint32_t event_mask;
    mutable bool opened;

   public:
    std::list<std::unique_ptr<Window>> subwindows;

    Window();
    virtual ~Window();

    virtual void open() = 0;
    virtual void close() = 0;

    bool is_opened() const;

    void set_event_mask(uint32_t mask);
    void add_child_window(std::unique_ptr<Window> child);
    //virtual void handle_event(Event event) = 0;
};

class RenderWindow : public Window {
   protected:
    Position pos;
    Size size;
    float rotation;

   public:
    RenderWindow();

    virtual ~RenderWindow();

    RenderWindow(const RenderWindow& other) = delete;

    RenderWindow(Size size, Position pos);

    void open();

    void close();

    void set_pos(Position pos);

    virtual Position get_position() const;

    void set_rotation(const float rotation);

    float get_rotation() const;

    virtual void render(Renderer& target) = 0;
};

class RectWindow : public RenderWindow {
   protected:
       Color color;

   public:
    RectWindow();
    ~RectWindow();
    RectWindow(Size size, Position pos, Color color);

    virtual void render(Renderer& target) override;
};

#endif
