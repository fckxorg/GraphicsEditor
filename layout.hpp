/* LAYOUT DEFENITION */
CREATE(root_window, RootWindow);
CREATE(window, RectWindow, Size(1920, 1080), Position(0, 0),
       Color(212, 212, 212));

CREATE(toolbar_listener, ToolbarListener);

CREATE(pencil_button_sprite, Sprite, Texture("icons/pencil.png", Size(50, 50)),
       Position(15, 910));
CREATE(eraser_button_sprite, Sprite, Texture("icons/eraser.png", Size(50, 50)),
       Position(15, 980));
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

/* SUBSCRIPTIONS */

SUBSCRIBE(root_window.get(), sv_fader.get());
SUBSCRIBE(root_window.get(), thickness_slider.get());
SUBSCRIBE(root_window.get(), hue_slider.get());
SUBSCRIBE(root_window.get(), pencil_button.get());
SUBSCRIBE(root_window.get(), eraser_button.get());
SUBSCRIBE(root_window.get(), save_button.get());
SUBSCRIBE(root_window.get(), canvas.get());

SUBSCRIBE(hue_selector.get(), sv_fader.get());
SUBSCRIBE(hue_selector.get(), sv_selector.get());
SUBSCRIBE(sv_selector.get(), toolbar_listener.get());

SUBSCRIBE(thickness_slider.get(), toolbar_listener.get());
SUBSCRIBE(hue_slider.get(), hue_selector.get());
SUBSCRIBE(sv_fader.get(), sv_selector.get());

SUBSCRIBE(pencil_button.get(), toolbar_listener.get());
SUBSCRIBE(eraser_button.get(), toolbar_listener.get());
SUBSCRIBE(save_button.get(), canvas.get());

/* ADOPTIONS */

ADOPT(pencil_button, pencil_button_sprite);
ADOPT(eraser_button, eraser_button_sprite);
ADOPT(save_button, save_button_sprite);

ADOPT(sv_selector, sv_fader);

ADOPT(pencil_button_outline, pencil_button);
ADOPT(eraser_button_outline, eraser_button);
ADOPT(hue_selector_outline, hue_selector);
ADOPT(hue_selector_outline, hue_slider);
ADOPT(sv_selector_outline, sv_selector);
ADOPT(save_button_outline, save_button);

ADOPT(thickness_slider_base, thickness_slider);

ADOPT(root_window, window);
ADOPT(root_window, pencil_button_outline);
ADOPT(root_window, eraser_button_outline);
ADOPT(root_window, hue_selector_outline);
ADOPT(root_window, sv_selector_outline);
ADOPT(root_window, save_button_outline);
ADOPT(root_window, thickness_slider_base);
ADOPT(root_window, canvas);
