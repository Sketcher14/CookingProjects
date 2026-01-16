cmake_minimum_required(VERSION 3.24)
project(MultithreadingLab_Main)

add_executable(MultithreadingLab_Main
        main.cpp
        accumulation.h
        accumulation.cpp
        thread_pool.h
        thread_pool.cpp
)

#target_compile_options(MultithreadingLab_Main PRIVATE -fsanitize=address -fno-omit-frame-pointer)
#target_link_options(MultithreadingLab_Main PRIVATE -fsanitize=address)
#target_compile_options(MultithreadingLab_Main PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
#target_link_options(MultithreadingLab_Main PRIVATE -fsanitize=thread)