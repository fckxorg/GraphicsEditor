#include <iostream>
#include <unistd.h>

#include "data_classes/data_classes.hpp"
#include "window/window.hpp"


int main() {
    Renderer renderer (Size(1920, 1080));

    RectWindow window(Size(1920, 900), Position(0, 0), Color(176, 176, 176));
   
    window.open(); 
    window.render(renderer);

    while(true) {
        renderer.show();
        sleep(10);
    }

    return 0;
}

