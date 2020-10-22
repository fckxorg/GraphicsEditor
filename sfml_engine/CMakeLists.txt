add_library(sfml_engine sfml_engine.hpp sfml_engine.cpp)

target_include_directories(sfml_engine 
                          PUBLIC "${PROJECT_SOURCE_DIR}/data_classes")
target_link_libraries(sfml_engine PUBLIC data_classes)
set_target_properties(sfml_engine PROPERTIES LINKER_LANGUAGE CXX)
