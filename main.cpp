#include <iostream>
#include <memory>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "ncurses_engine/ncurses_engine.hpp"
#include "window/window.hpp"


int main() {
    
    char roboto_font_path[] = "fonts/Roboto-Thin.ttf";

    RectWindow window(Size(1920, 900), Position(0, 0), Color(255, 255, 255));
  
    Text button_text = {"Test", 12, roboto_font_path, Color(255, 0, 0)};
    std::unique_ptr<Window> button_text_window(new TextWindow(button_text, Position(765, 333), Color(0, 240, 255)));
    button_text_window->open();

    std::unique_ptr<Window> test_button(new RectButton(Size(100, 40), Position(765, 333), Color(0, 240, 255)));
    test_button->open();
    test_button->add_child_window(button_text_window);
    window.add_child_window(test_button);

    window.open(); 
    Renderer::init(Size(1920, 1080));
    window.render();

    while(true) {
        Renderer::show();
    }

    Renderer::deinit();

    return 0;
}

