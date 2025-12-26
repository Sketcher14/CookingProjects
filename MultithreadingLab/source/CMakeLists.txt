cmake_minimum_required(VERSION 3.24)
project(MultithreadingLab_Main)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fexperimental-library)

add_executable(MultithreadingLab_Main
        main.cpp
        accumulation.h
        accumulation.cpp
)

target_link_libraries(MultithreadingLab_Main PUBLIC Utility)