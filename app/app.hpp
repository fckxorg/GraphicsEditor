#ifndef APP_HPP
#define APP_HPP
#include <queue>
#include <memory>

#include "../event/event.hpp"
#include "../window/window.hpp"

template<typename RenderEngine>
class App {
    private:
        std::queue<Event> event_queue;
        std::unique_ptr<Window> root_window;
    public:
        App();
        ~App();

        void event_loop();
};

#endif
