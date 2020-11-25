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

int main() {
  FILE* test_file = fopen("test_text.txt", "r");

  if (test_file == nullptr) {
    perror("Can't open the file");
    return -1;
  }

  /* LAYOUT DEFENITION */
  CREATE(root_window, RootWindow);
  CREATE(window, RectWindow, Size(1920, 1080), Position(0, 0),
         Color(212, 212, 212));

  CREATE(toolbar_listener, ToolbarListener);

  CREATE(pencil_button_sprite, Sprite,
         Texture("icons/pencil.png", Size(50, 50)), Position(15, 910));
  CREATE(eraser_button_sprite, Sprite,
         Texture("icons/eraser.png", Size(50, 50)), Position(15, 980));
  CREATE(save_button_sprite, Sprite, Texture("icons/save.png", Size(50, 50)),
         Position(1515, 15));

  CREATE(pencil_button_outline, RectWindow, Size(60, 60), Position(10, 905),
         Color(80, 90, 91));
  CREATE(eraser_button_outline, RectWindow, Size(60, 60), Position(10, 975),
         Color(80, 90, 91));
  CREATE(hue_selector_outline, RectWindow, Size(370, 40), Position(1540, 1010),
         Color(80, 90, 91));
  CREATE(sv_selector_outline, RectWindow, Size(370, 370), Position(1540, 630),
         Color(80, 90, 91));
  CREATE(save_button_outline, RectWindow, Size(60, 60), Position(1510, 10),
         Color(80, 90, 91));

  CREATE(pencil_button, RectButton, Size(50, 50), Position(15, 910),
         Color(236, 236, 236), PENCIL);
  CREATE(eraser_button, RectButton, Size(50, 50), Position(15, 980),
         Color(236, 236, 236), ERASER);
  CREATE(save_button, RectButton, Size(50, 50), Position(1515, 15),
         Color(236, 236, 236), Canvas::SAVE);

  CREATE(hue_selector, HUEselector, Size(360, 30), Position(1545, 1015));
  CREATE(sv_selector, SVselector, Size(360, 360), Position(1545, 635));
  CREATE(thickness_slider_base, RectWindow, Size(360, 2), Position(1545, 609),
         Color(80, 90, 91));

  CREATE(hue_slider, Slider, Size(3, 30), Position(1545, 1015), Color(0, 0, 0),
         1545, 1905, 1, true);
  CREATE(thickness_slider, Slider, Size(5, 30), Position(1545, 595),
         Color(0, 0, 0), 1545, 1905, 1, true);
  CREATE(sv_fader, Fader, Size(8, 8), Position(1896, 986), Color(0, 0, 0),
         Position(1545, 635), Position(1896, 986));


  CREATE(canvas, Canvas, Size(1500, 890), Position(0, 0), Color(255, 255, 255));

  SUBSCRIBE(root_window.get(), sv_fader.get());
  SUBSCRIBE(hue_selector.get(), sv_fader.get());
  SUBSCRIBE(sv_fader.get(), sv_selector.get());
  SUBSCRIBE(sv_selector.get(), toolbar_listener.get());
  SUBSCRIBE(root_window.get(), thickness_slider.get());
  SUBSCRIBE(thickness_slider.get(), toolbar_listener.get());

  SUBSCRIBE(root_window.get(), hue_slider.get());

  pencil_button->add_child_window(pencil_button_sprite);
  eraser_button->add_child_window(eraser_button_sprite);
  save_button->add_child_window(save_button_sprite);

  SUBSCRIBE(root_window.get(), pencil_button.get());
  SUBSCRIBE(root_window.get(), eraser_button.get());
  SUBSCRIBE(root_window.get(), save_button.get());

  SUBSCRIBE(pencil_button.get(), toolbar_listener.get());
  SUBSCRIBE(eraser_button.get(), toolbar_listener.get());
  SUBSCRIBE(save_button.get(), canvas.get());

  SUBSCRIBE(hue_slider.get(), hue_selector.get());
  SUBSCRIBE(hue_selector.get(), sv_selector.get());

  sv_selector->add_child_window(sv_fader);

  pencil_button_outline->add_child_window(pencil_button);
  eraser_button_outline->add_child_window(eraser_button);
  hue_selector_outline->add_child_window(hue_selector);
  hue_selector_outline->add_child_window(hue_slider);
  sv_selector_outline->add_child_window(sv_selector);
  save_button_outline->add_child_window(save_button);
  thickness_slider_base->add_child_window(thickness_slider);

  InstrumentManager::init();

  SUBSCRIBE(root_window.get(), canvas.get());

  window->add_child_window(canvas);

  root_window->add_child_window(window);
  root_window->add_child_window(pencil_button_outline);
  root_window->add_child_window(eraser_button_outline);
  root_window->add_child_window(hue_selector_outline);
  root_window->add_child_window(sv_selector_outline);
  root_window->add_child_window(save_button_outline);
  root_window->add_child_window(thickness_slider_base);

  App::init(Size(1920, 1080), "Test application");
  App::set_root_window(root_window);
  App::run();
  App::deinit();

  return 0;
}

