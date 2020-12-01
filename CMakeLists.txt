cmake_minimum_required(VERSION 3.15)
project(WindowManager VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(Main main.cpp)


add_definitions(-DSFML_ENGINE)
add_subdirectory(sfml_engine)



add_subdirectory(data_classes)
add_subdirectory(window)
add_subdirectory(app)
add_subdirectory(event_queue)
add_subdirectory(event)
add_subdirectory(window_base)
add_subdirectory(subscription_manager)
add_subdirectory(instruments_manager)
add_subdirectory(color_utilities)

message(STATUS "Building with SFML")
target_include_directories(Main 
                          PUBLIC "${PROJECT_BINARY_DIR}"
                          PUBLIC "${PROJECT_SOURCE_DIR}/data_classes"                          
                          PUBLIC "${PROJECT_SOURCE_DIR}/window"
                          PUBLIC "${PROJECT_SOURCE_DIR}/app"
                          PUBLIC "${PROJECT_SOURCE_DIR}/event_queue"
                          PUBLIC "${PROJECT_SOURCE_DIR}/event"
                          PUBLIC "${PROJECT_SOURCE_DIR}/window_base"
                          PUBLIC "${PROJECT_SOURCE_DIR}/subscription_manager"
                          PUBLIC "${PROJECT_SOURCE_DIR}/sfml_engine"
                          PUBLIC "${PROJECT_SOURCE_DIR}/color_utilities"
                          PUBLIC "${PROJECT_SOURCE_DIR}/instruments_manager")
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/SFML/cmake-modules/")
find_package(SFML REQUIRED system window graphics)
target_link_libraries(Main PUBLIC sfml-system sfml-window sfml-graphics data_classes window_base window color_utilities instrument_manager subscription_manager sfml_engine app event_queue event)

