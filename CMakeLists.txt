cmake_minimum_required(VERSION 3.15)
project(WindowManager VERSION 1.0)

option(NCURSES_ENGINE "Use ncurses as render engine" ON)
option(SFML_ENGINE "Use SFML as render engine" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(Main main.cpp)


if(SFML_ENGINE)
    add_definitions(-DSFML_ENGINE)
    add_subdirectory(sfml_engine)
endif(SFML_ENGINE)


if(NCURSES_ENGINE)
    add_definitions(-DNCURSES_ENGINE)
    add_subdirectory(ncurses_engine)
endif(NCURSES_ENGINE)

add_subdirectory(data_classes)
add_subdirectory(window)

if(NCURSES_ENGINE)
    message(STATUS "Builidng with ncurses")
    find_package(Curses REQUIRED)

    target_include_directories(Main 
                              PUBLIC "${PROJECT_BINARY_DIR}"
                              PUBLIC "${PROJECT_SOURCE_DIR}/data_classes"                          
                              PUBLIC "${PROJECT_SOURCE_DIR}/window"
                              PUBLIC "${PROJECT_SOURCE_DIR}/ncurses_engine"
                              PUBLIC "${CURSES_INCLUDE_DIR}")

    include_directories(${CURSES_INCLUDE_DIR})

    target_link_libraries(Main PUBLIC ${CURSES_LIBRARIES} data_classes window ncurses_engine)
endif(NCURSES_ENGINE)

if(SFML_ENGINE)
    message(STATUS "Building with SFML")
    target_include_directories(Main 
                              PUBLIC "${PROJECT_BINARY_DIR}"
                              PUBLIC "${PROJECT_SOURCE_DIR}/data_classes"                          
                              PUBLIC "${PROJECT_SOURCE_DIR}/window"
                              PUBLIC "${PROJECT_SOURCE_DIR}/sfml_engine")
    set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/SFML/cmake-modules/")
    find_package(SFML REQUIRED system window graphics)
    target_link_libraries(Main PUBLIC sfml-system sfml-window sfml-graphics data_classes window sfml_engine)
endif(SFML_ENGINE)

