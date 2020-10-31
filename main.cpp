#include <bits/stdint-uintn.h>
#include <iostream>
#include <memory>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "window/window.hpp"
#include "app/app.hpp"
#include "subscription_manager/subscription_manager.hpp"


int main() {
   
    FILE* test_file = fopen("test_text.txt", "r");
    fseek(test_file, 0, SEEK_END);
    uint64_t file_size = ftell(test_file);
    rewind(test_file);

    std::unique_ptr<char> text_buffer = std::unique_ptr<char>(new char[file_size]());
    fread(text_buffer.get(), file_size, sizeof(char), test_file);

    fclose(test_file);
     
    char roboto_font_path[] = "fonts/Roboto-Thin.ttf";
    
    std::unique_ptr<Window> root_window(new RootWindow());    
    std::unique_ptr<Window> window (new RectWindow(Size(1920, 900), Position(0, 0), Color(255, 255, 255)));
  
    Text scroll_test = {text_buffer.get(), 18, roboto_font_path, Color(0, 0, 0)};

    std::unique_ptr<Window> scrollable_text(new ScrollableText(Size(380, 400), Position(100, 100), Color(0, 240, 255), scroll_test));

    std::unique_ptr<Window> scrollbar(new Scrollbar(Size(30, 400), Position(480, 100), Color(245, 245, 245), false));
    
    // should be subscribed before ownership moves to window in hieararchy
    SubscriptionManager::add_subscription(root_window.get(), scrollbar.get());
    SubscriptionManager::add_subscription(dynamic_cast<Scrollbar*>(scrollbar.get())->slider_ptr, scrollable_text.get());

    window->add_child_window(scrollbar);
    window->add_child_window(scrollable_text);

    root_window->add_child_window(window);
  

    App::init(Size(1920, 1080), "Test application");
    App::set_root_window(root_window);
    App::run();
    App::deinit();   

    return 0;
}

