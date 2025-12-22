cmake_minimum_required(VERSION 3.24)
project(Utility)

set(CMAKE_CXX_STANDARD 20)

add_library(Utility INTERFACE
        print.h
)

target_include_directories(Utility INTERFACE .)
