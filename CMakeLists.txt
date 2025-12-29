cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(CookingProjects)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(Utility)

add_subdirectory(MultithreadingLab)
add_subdirectory(Sandbox)
add_subdirectory(SortLab)