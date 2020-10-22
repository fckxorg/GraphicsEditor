#include <iostream>
#include <memory>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "window/window.hpp"


int main() {

    RectWindow window(Size(1920, 900), Position(0, 0), Color(176, 176, 176));
  
    /*Text button_text = {"Test", 12, NULL, Color(255, 0, 0)};
    TextWindow* button_window = new TextWindow(button_text);
    button_window->open();
    button_window->set_pos(Position(765, 333));
    button_window->set_bgcolor(Color(0, 255, 255));
*/
    std::unique_ptr<Window> test_button(new RectWindow(Size(100, 20), Position(765, 333), Color(0, 255, 0)));
    test_button->open();
    // test_button->add_child_window(std::unique_ptr<Window>(button_window));
    window.add_child_window(test_button);

    window.open(); 
    Renderer renderer (Size(1920, 1080));
    window.render(renderer);

    while(true) {
        renderer.show();
        sleep(10);
    }

    return 0;
}

