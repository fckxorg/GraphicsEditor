//          TYPE            SFML_ENGINE_CODE                                                                    NCURSES_ENGINE_CODE
WINDOW_TYPE(ABSTRACT_WINDOW, ;,                                                                                 ;)
WINDOW_TYPE(RENDER_WIDNOW,   ;,                                                                                 ;)
WINDOW_TYPE(RECT_WINDOW,     sf::RectangleShape rect_window;
                             Size window_size = window->get_size();
                             Color window_color = window->get_color();
                             Position window_position = window->get_position();
                             sf::Color sf_color = sf::Color(window_color.r, window_color.g, window_color.b);
                             rect_window.setSize(window_size.width, window_size.height);
                             rect_window.setPosition(widnow_position.x, window_position.y);
                             rect_window.setFillColor(sf_color);

