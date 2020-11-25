#include <bits/stdint-uintn.h>
#include <unistd.h>

#include <iostream>
#include <memory>

#include "app/app.hpp"
#include "data_classes/data_classes.hpp"
#include "instruments_manager/instruments_manager.hpp"
#include "subscription_manager/subscription_manager.hpp"
#include "window/window.hpp"

#define CREATE(WINDOW_NAME, WINDOW_TYPE, ...) \
  std::unique_ptr<Window> WINDOW_NAME(new WINDOW_TYPE(__VA_ARGS__))

#define ADOPT(PARENT, CHILD) (PARENT)->add_child_window((CHILD))

int main() {
 
  #include "layout.hpp"

  InstrumentManager::init();
  App::init(Size(1920, 1080), "Test application");
  App::set_root_window(root_window);
  App::run();
  App::deinit();

  return 0;
}

