cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(Sandbox
        main.cpp
        cubes/string.h
        cubes/string.cpp
)

target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})