cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(process_utils VERSION 1.0.0 LANGUAGES CXX)

add_library(process_utils SHARED process_utils.cpp process_utils.def)
target_link_libraries(process_utils -static-libstdc++)

set_target_properties(process_utils PROPERTIES
    PUBLIC_HEADER process_utils.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "_process_utils"
)