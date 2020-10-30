#include <iostream>
#include <memory>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "window/window.hpp"
#include "app/app.hpp"
#include "subscription_manager/subscription_manager.hpp"


int main() {
    
    char roboto_font_path[] = "fonts/Roboto-Thin.ttf";
    
    std::unique_ptr<Window> root_window(new RootWindow());    

    std::unique_ptr<Window> window (new RectWindow(Size(1920, 900), Position(0, 0), Color(255, 255, 255)));
  
    Text button_text = {"Test", 12, roboto_font_path, Color(255, 0, 0)};
    std::unique_ptr<Window> button_text_window(new TextWindow(button_text, Position(765, 333), Color(0, 240, 255)));

    std::unique_ptr<Window> test_button(new RectButton(Size(100, 40), Position(765, 333), Color(0, 240, 255)));
    test_button->add_child_window(button_text_window);

    std::unique_ptr<Window> scrollbar(new Scrollbar(Size(30, 400), Position(400, 400), Color(245, 245, 245), false));
    
    // should be subscribed before ownership moves to window in hieararchy
    SubscriptionManager::add_subscription(root_window.get(), test_button.get());
    SubscriptionManager::add_subscription(root_window.get(), scrollbar.get());

    window->add_child_window(test_button);
    window->add_child_window(scrollbar);

    root_window->add_child_window(window);
  

    App::init(Size(1920, 1080), "Test application");
    App::set_root_window(root_window);
    App::run();
    App::deinit();   

    return 0;
}

