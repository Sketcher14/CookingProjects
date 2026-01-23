cmake_minimum_required(VERSION 4.0 FATAL_ERROR)
project(CookingProjects)

set(CMAKE_CXX_STANDARD 23)
if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wconversion)
endif ()

add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)
#add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
#add_link_options(-fsanitize=thread)

add_subdirectory(Utility)

add_subdirectory(MultithreadingLab)
add_subdirectory(Sandbox)
add_subdirectory(SortLab)