project(Sandbox)

add_executable(Sandbox
        main.cpp
        cubes/string.h
        cubes/string.cpp
)
target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})