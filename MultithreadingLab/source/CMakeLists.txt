project(MultithreadingLab_Main)

# Creating a library with algorithms and data structures
add_library(MultithreadingLab_Library STATIC
        accumulation.h
        accumulation.cpp
        thread_pool.h
        thread_pool.cpp
        timer_manager.h
        timer_manager.cpp
)
target_include_directories(MultithreadingLab_Library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Adding an executable for this target
add_executable(MultithreadingLab_Main  main.cpp)
target_link_libraries(MultithreadingLab_Main PRIVATE MultithreadingLab_Library)