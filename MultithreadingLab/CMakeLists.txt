cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(MultithreadingLab)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-fsanitize=thread)
add_link_options(-fsanitize=thread)

add_subdirectory(source)