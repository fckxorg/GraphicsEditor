#include <iostream>
#include <unistd.h>

#include "sfml_engine/sfml_engine.hpp"
#include "window/window.hpp"


int main() {
    Renderer renderer (Size(800, 800));

    RectWindow window(Size(300, 300), Position(100, 100), Color(255, 0, 0));
   
    window.open(); 
    window.render(renderer);

    while(true) {
        renderer.show();
        sleep(10);
    }

    return 0;
}

