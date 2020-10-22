#include <iostream>
#include <unistd.h>

#include "window/window.hpp"


int main() {
    Renderer renderer (Size(300, 300));

    RectWindow window(Size(50, 20), Position(10, 10), Color(255, 0, 0));
   
    window.open(); 
    window.render(renderer);

    while(true) {
        renderer.show();
        sleep(10);
    }

    return 0;
}

