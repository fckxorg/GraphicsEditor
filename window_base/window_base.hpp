#include <list>
#include <memory>
#include <cstdint>

#include "../event/event.hpp"

class Window {
   protected:
    uint32_t event_mask;

   public:
    std::list<std::unique_ptr<Window>> subwindows;

    Window();
    virtual ~Window();

    bool is_opened() const;

    void set_event_mask(uint32_t mask);
    void add_child_window(std::unique_ptr<Window>& child);
    virtual void handle_event(Event* event);
    virtual void render() = 0;
};


