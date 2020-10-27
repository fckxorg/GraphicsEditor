#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <bits/stdint-uintn.h>
#include <stdio.h>

#include <cstdint>
#include <list>
#include <memory>

#include "../data_classes/data_classes.hpp"

#ifdef NCURSES_ENGINE
#include "../ncurses_engine/ncurses_engine.hpp"
#endif

#ifdef SFML_ENGINE
#include "../sfml_engine/sfml_engine.hpp"
#endif

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

    virtual void refresh() = 0;

    bool is_opened() const;

    void set_event_mask(uint32_t mask);
    void add_child_window(std::unique_ptr<Window>& child);
    // virtual void handle_event(Event event) = 0;
    virtual void render() = 0;
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

    virtual void refresh() override;

    void set_pos(Position pos);

    virtual Position get_position() const;

    void set_rotation(const float rotation);

    float get_rotation() const;
};

class RectWindow : public RenderWindow {
   protected:
    Color color;

   public:
    RectWindow();
    ~RectWindow();
    RectWindow(Size size, Position pos, Color color);

    virtual void render() override;
};

class TextWindow : public RenderWindow {
    private:
        Color bgcolor;
        Text text;
    public:
        TextWindow();
        TextWindow(Text text, Position pos, Color bgcolor);

        Text get_text() const;
        void set_text(Text text);

        void set_bgcolor(Color bgcolor); 
        Color get_bgcolor() const;

        virtual void render() override;
};

class RectButton : public RectWindow {
   public:
    RectButton();
    ~RectButton();

    virtual void render() override;
    RectButton(Size size, Position pos, Color color) : RectWindow(size, pos, color) {}

    void on_hover();

    void on_click();
};

#endif
