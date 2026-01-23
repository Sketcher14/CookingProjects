project(Utility)

add_library(Utility_Test STATIC
        utility.h
        utility.cpp
)

target_include_directories(Utility_Test PUBLIC .)
