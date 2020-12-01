/* LAYOUT DEFENITION */
#include "macro.hpp"

const Color OUTLINE_COLOR = Color(80, 90, 91);
const Color EDITOR_BUTTON_COLOR = Color(236, 236, 236);
const Size EDITOR_BUTTON_OUTLINE_SIZE = Size(60, 60);
const Size EDITOR_BUTTON_SIZE = Size(50, 50);

CREATE(root_window, RootWindow);
CREATE(window, RectWindow, Size(1920, 1080), Position(0, 0),
       Color(212, 212, 212));

CREATE(toolbar_listener, ToolbarListener);

CREATE(pencil_button_sprite, Sprite, Texture("icons/pencil.png", EDITOR_BUTTON_SIZE),
       Position(15, 910));
CREATE(eraser_button_sprite, Sprite, Texture("icons/eraser.png", EDITOR_BUTTON_SIZE),
       Position(15, 980));
CREATE(save_button_sprite, Sprite, Texture("icons/save.png", EDITOR_BUTTON_SIZE),
       Position(1515, 15));
CREATE(brush_button_sprite, Sprite, Texture("icons/brush.png", EDITOR_BUTTON_SIZE),
       Position(85, 910));
CREATE(dropper_button_sprite, Sprite,
       Texture("icons/dropper.png", EDITOR_BUTTON_SIZE), Position(85, 980));
CREATE(spray_button_sprite, Sprite, Texture("icons/spray.png", Size(50, 50)),
       Position(155, 910));
CREATE(clear_button_sprite, Sprite, Texture("icons/clear.png", EDITOR_BUTTON_SIZE),
       Position(155, 980));
CREATE(rect_button_sprite, Sprite,
       Texture("icons/rectangular.png", EDITOR_BUTTON_SIZE), Position(225, 910));
CREATE(ellipse_button_sprite, Sprite,
       Texture("icons/ellipse.png", EDITOR_BUTTON_SIZE), Position(225, 980));

CREATE(pencil_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(10, 905),
       OUTLINE_COLOR);
CREATE(eraser_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(10, 975),
       OUTLINE_COLOR);
CREATE(hue_selector_outline, RectWindow, Size(370, 40), Position(1540, 1010),
       OUTLINE_COLOR);
CREATE(sv_selector_outline, RectWindow, Size(370, 370), Position(1540, 630),
       OUTLINE_COLOR);
CREATE(save_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(1510, 10),
       OUTLINE_COLOR);
CREATE(brush_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(80, 905),
       OUTLINE_COLOR);
CREATE(dropper_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(80, 975),
       OUTLINE_COLOR);
CREATE(spray_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(150, 905),
       OUTLINE_COLOR);
CREATE(clear_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(150, 975),
       OUTLINE_COLOR);
CREATE(rect_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(220, 905),
       OUTLINE_COLOR);
CREATE(ellipse_button_outline, RectWindow, EDITOR_BUTTON_OUTLINE_SIZE, Position(220, 975),
       OUTLINE_COLOR);

CREATE(pencil_button, RectButton, EDITOR_BUTTON_SIZE, Position(15, 910),
       EDITOR_BUTTON_COLOR, PENCIL);
CREATE(eraser_button, RectButton, EDITOR_BUTTON_SIZE, Position(15, 980),
       EDITOR_BUTTON_COLOR, ERASER);
CREATE(save_button, SaveButton, EDITOR_BUTTON_SIZE, Position(1515, 15),
       EDITOR_BUTTON_COLOR);
CREATE(brush_button, RectButton, EDITOR_BUTTON_SIZE, Position(85, 910),
       EDITOR_BUTTON_COLOR, BRUSH);
CREATE(dropper_button, RectButton, EDITOR_BUTTON_SIZE, Position(85, 980),
       EDITOR_BUTTON_COLOR, DROPPER);
CREATE(spray_button, RectButton, EDITOR_BUTTON_SIZE, Position(155, 910),
       EDITOR_BUTTON_COLOR, SPRAY);
CREATE(clear_button, RectButton, EDITOR_BUTTON_SIZE, Position(155, 980),
       EDITOR_BUTTON_COLOR, CLEAR);
CREATE(rect_button, RectButton, EDITOR_BUTTON_SIZE, Position(225, 910),
       EDITOR_BUTTON_COLOR, RECT_INSTRUMENT);
CREATE(ellipse_button, RectButton, EDITOR_BUTTON_SIZE, Position(225, 980),
       EDITOR_BUTTON_COLOR, ELLIPSE_INSTRUMENT);

CREATE(hue_selector, HUEselector, Size(360, 30), Position(1545, 1015));
CREATE(sv_selector, SVselector, Size(360, 360), Position(1545, 635));
CREATE(thickness_slider_base, RectWindow, Size(360, 2), Position(1545, 609),
       Color(80, 90, 91));

CREATE(hue_slider, HueSlider, Size(3, 30), Position(1545, 1015), Color(0, 0, 0),
       1545, 1905, 1, true);
CREATE(thickness_slider, Slider, Size(5, 30), Position(1545, 595),
       Color(0, 0, 0), 1545, 1905, 1, true);

CREATE(sv_fader, SVFader, Size(8, 8), Position(1896, 986), Color(0, 0, 0),
       Position(1545, 635), Position(1896, 986));

CREATE(canvas, Canvas, Size(1500, 890), Position(0, 0), Color(255, 255, 255));

/* SUBSCRIPTIONS */

SUBS(root_window, sv_fader);
SUBS(root_window, thickness_slider);
SUBS(root_window, hue_slider);
SUBS(root_window, pencil_button);
SUBS(root_window, eraser_button);
SUBS(root_window, save_button);
SUBS(root_window, canvas);
SUBS(root_window, brush_button);
SUBS(root_window, dropper_button);
SUBS(root_window, spray_button);
SUBS(root_window, clear_button);
SUBS(root_window, rect_button);
SUBS(root_window, ellipse_button);

SUBS(hue_selector, sv_fader);
SUBS(hue_selector, sv_selector);
SUBS(sv_selector, toolbar_listener);

SUBS(thickness_slider, toolbar_listener);
SUBS(hue_slider, hue_selector);
SUBS(sv_fader, sv_selector);

SUBS(pencil_button, toolbar_listener);
SUBS(eraser_button, toolbar_listener);
SUBS(save_button, canvas);
SUBS(brush_button, toolbar_listener);
SUBS(dropper_button, toolbar_listener);
SUBS(spray_button, toolbar_listener);
SUBS(clear_button, toolbar_listener);
SUBS(rect_button, toolbar_listener);
SUBS(ellipse_button, toolbar_listener);

/* ADOPTIONS */

ADOPT(pencil_button, pencil_button_sprite);
ADOPT(eraser_button, eraser_button_sprite);
ADOPT(save_button, save_button_sprite);
ADOPT(brush_button, brush_button_sprite);
ADOPT(dropper_button, dropper_button_sprite);
ADOPT(spray_button, spray_button_sprite);
ADOPT(clear_button, clear_button_sprite);
ADOPT(rect_button, rect_button_sprite);
ADOPT(ellipse_button, ellipse_button_sprite);

ADOPT(sv_selector, sv_fader);

ADOPT(pencil_button_outline, pencil_button);
ADOPT(eraser_button_outline, eraser_button);
ADOPT(hue_selector_outline, hue_selector);
ADOPT(hue_selector_outline, hue_slider);
ADOPT(sv_selector_outline, sv_selector);
ADOPT(save_button_outline, save_button);
ADOPT(brush_button_outline, brush_button);
ADOPT(dropper_button_outline, dropper_button);
ADOPT(spray_button_outline, spray_button);
ADOPT(clear_button_outline, clear_button);
ADOPT(rect_button_outline, rect_button);
ADOPT(ellipse_button_outline, ellipse_button);

ADOPT(thickness_slider_base, thickness_slider);

ADOPT(window, pencil_button_outline);
ADOPT(window, eraser_button_outline);
ADOPT(window, brush_button_outline);
ADOPT(window, hue_selector_outline);
ADOPT(window, sv_selector_outline);
ADOPT(window, thickness_slider_base);
ADOPT(window, dropper_button_outline);
ADOPT(window, spray_button_outline);
ADOPT(window, clear_button_outline);
ADOPT(window, canvas);
ADOPT(window, rect_button_outline);
ADOPT(window, ellipse_button_outline);
ADOPT(window, save_button_outline);
ADOPT(root_window, window);
