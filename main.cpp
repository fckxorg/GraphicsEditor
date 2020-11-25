#include <bits/stdint-uintn.h>
#include <unistd.h>

#include <iostream>
#include <memory>

#include "app/app.hpp"
#include "data_classes/data_classes.hpp"
#include "instruments_manager/instruments_manager.hpp"
#include "subscription_manager/subscription_manager.hpp"
#include "window/window.hpp"

int main() {
  FILE* test_file = fopen("test_text.txt", "r");

  if (test_file == nullptr) {
    perror("Can't open the file");
    return -1;
  }

  std::unique_ptr<Window> root_window(new RootWindow());
  std::unique_ptr<Window> window(
      new RectWindow(Size(1920, 1080), Position(0, 0), Color(212, 212, 212)));

  std::unique_ptr<Window> pencil_button_sprite(new Sprite(
      Texture("icons/pencil.png", Size(50, 50)), Position(320, 910)));
  std::unique_ptr<Window> eraser_button_sprite(new Sprite(
      Texture("icons/eraser.png", Size(50, 50)), Position(320, 980)));
  std::unique_ptr<Window> color_picker_sprite(new Sprite(
      Texture("icons/colorpicker.png", Size(300, 300)), Position(10, 900)));

  std::unique_ptr<Window> toolbar_listener(new ToolbarListener());

  std::unique_ptr<Window> pencil_button_outline(
      new RectWindow(Size(60, 60), Position(315, 905), Color(80, 90, 91)));
  std::unique_ptr<Window> eraser_button_outline(
      new RectWindow(Size(60, 60), Position(315, 975), Color(80, 90, 91)));

  std::unique_ptr<Window> pencil_button(new RectButton(
      Size(50, 50), Position(320, 910), Color(236, 236, 236), PENCIL));
  std::unique_ptr<Window> eraser_button(new RectButton(
      Size(50, 50), Position(320, 980), Color(236, 236, 236), ERASER));

  pencil_button->add_child_window(pencil_button_sprite);
  eraser_button->add_child_window(eraser_button_sprite);

  SUBSCRIBE(root_window.get(), pencil_button.get());
  SUBSCRIBE(root_window.get(), eraser_button.get());

  SUBSCRIBE(pencil_button.get(), toolbar_listener.get());
  SUBSCRIBE(eraser_button.get(), toolbar_listener.get());

  pencil_button_outline->add_child_window(pencil_button);
  eraser_button_outline->add_child_window(eraser_button);

  InstrumentManager::init();

  std::unique_ptr<Window> canvas(
      new Canvas(Size(1920, 890), Position(0, 0), Color(255, 255, 255)));

  SUBSCRIBE(root_window.get(), canvas.get());

  window->add_child_window(canvas);
  window->add_child_window(color_picker_sprite);

  root_window->add_child_window(window);
  root_window->add_child_window(pencil_button_outline);
  root_window->add_child_window(eraser_button_outline);

  App::init(Size(1920, 1080), "Test application");
  App::set_root_window(root_window);
  App::run();
  App::deinit();

  return 0;
}

