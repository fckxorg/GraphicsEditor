#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <bits/stdint-uintn.h>
#include <stdio.h>

#include <cstdint>
#include <list>
#include <memory>

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"

#ifdef NCURSES_ENGINE
#include "../ncurses_engine/ncurses_engine.hpp"
#endif

#ifdef SFML_ENGINE
#include "../sfml_engine/sfml_engine.hpp"
#endif

class Window {
   protected:
    uint32_t event_mask;

   public:
    std::list<std::unique_ptr<Window>> subwindows;

    Window();
    virtual ~Window();

    virtual void refresh() = 0;

    bool is_opened() const;

    void set_event_mask(uint32_t mask);
    void add_child_window(std::unique_ptr<Window>& child);
    virtual void handle_event(Event& event);
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

    void set_color(Color color); 
    
    Color get_color();

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
   private:
       Color default_color;
   public:
    RectButton();
    ~RectButton();

    virtual void render() override;
    RectButton(Size size, Position pos, Color color);

    virtual void handle_event(Event& event) override;

    bool check_boundaries(Position click_pos);

    void on_hover();

    virtual void onMousePress(Event& event);
    virtual void onMouseRelease(Event& event);
};

class Slider : public RectWindow {
    private:
       Color default_color; 
       Position last_mouse_pos; 
       bool pressed;
       
       bool horizontal; 
       uint16_t lower_bound;
       uint16_t upper_bound;

       bool check_boundaries(Position click_pos);
    public:
        Slider();
        ~Slider();
        Slider(Size size, Position pos, Color color, uint16_t lower_bound, uint16_t upper_bound, bool horizontal=false);
        virtual void onMousePress(Event& event);
        virtual void onMouseRelease(Event& event);
        virtual void onMouseMove(Event& event);

        virtual void handle_event(Event& event);
};


class Scrollbar : public RectWindow {
    private:
        bool horizontal;
    public:
        Scrollbar();
        ~Scrollbar();

        Scrollbar(Size size, Position pos, Color color, bool horizontal = false);
};

#endif
