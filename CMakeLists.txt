cmake_minimum_required(VERSION 3.15)
project(WindowManager VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(data_classes)
add_subdirectory(window)
add_subdirectory(sfml_engine)

add_executable(Main main.cpp)

target_include_directories(Main 
                          PUBLIC "${PROJECT_BINARY_DIR}"
                          PUBLIC "${PROJECT_SOURCE_DIR}/data_classes"                          
                          PUBLIC "${PROJECT_SOURCE_DIR}/window"
                          PUBLIC "${PROJECT_SOURCE_DIR}/sfml_engine")

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/SFML/cmake-modules/")
find_package(SFML REQUIRED system window graphics)

target_link_libraries(Main PUBLIC sfml-system sfml-window sfml-graphics window sfml_engine data_classes)

