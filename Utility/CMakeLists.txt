cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(Utility)

set(CMAKE_CXX_STANDARD 23)

add_library(Utility_Test STATIC
        utility.h
        utility.cpp
)

target_include_directories(Utility_Test PUBLIC .)
