#ifndef APP_HPP
#define APP_HPP
#include <memory>

#ifdef NCURSES_ENGINE
#include "../ncurses_engine/ncurses_engine.hpp"
#endif

#ifdef SFML_ENGINE
#include "../sfml_engine/sfml_engine.hpp"
#endif

#include "../data_classes/data_classes.hpp"
#include "../event/event.hpp"
#include "../event_queue/event_queue.hpp"
#include "../window/window.hpp"

class App {
   private:
    static std::unique_ptr<Window> root_window;

   public:
    App() = delete;

    static void init(Size size, const char* name);
    static void run();
    static void deinit();
    static void set_root_window(std::unique_ptr<Window>& window);
};

#endif
