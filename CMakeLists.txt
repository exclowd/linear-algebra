cmake_minimum_required(VERSION 3.16)
project(la CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(la_include_directories INTERFACE)
target_include_directories(la_include_directories
        BEFORE
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_subdirectory(example)
add_subdirectory(test/la)
add_subdirectory(bench)
