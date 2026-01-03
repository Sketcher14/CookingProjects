cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)
project(SortLab)

set(CMAKE_CXX_STANDARD 23)

# Suppress FindBoost deprecation warning (CMP0167)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()
find_package(Boost 1.90 CONFIG REQUIRED)

add_subdirectory(source)
add_subdirectory(tests)