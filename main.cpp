#include <iostream>
#include <memory>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "window/window.hpp"
#include "app/app.hpp"


int main() {
    
    char roboto_font_path[] = "fonts/Roboto-Thin.ttf";

    std::unique_ptr<Window> window (new RectWindow(Size(1920, 900), Position(0, 0), Color(255, 255, 255)));
  
    Text button_text = {"Test", 12, roboto_font_path, Color(255, 0, 0)};
    std::unique_ptr<Window> button_text_window(new TextWindow(button_text, Position(765, 333), Color(0, 240, 255)));
    button_text_window->open();

    std::unique_ptr<Window> test_button(new RectButton(Size(100, 40), Position(765, 333), Color(0, 240, 255)));
    test_button->open();
    test_button->add_child_window(button_text_window);

    std::unique_ptr<Window> scrollbar(new Scrollbar(Size(30, 400), Position(400, 400), Color(245, 245, 245), false));
    scrollbar->open();
    window->add_child_window(test_button);
    window->add_child_window(scrollbar);

    window->open(); 
    App::init(Size(1920, 1080), "Test application");
    App::set_root_window(window);
    App::run();
    App::deinit();   

    return 0;
}

