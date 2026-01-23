project(Sandbox)

add_executable(Sandbox
        main.cpp
        cubes/string.h
        cubes/string.cpp
        classes/my_heap.h
        classes/my_heap.cpp
        classes/my_forward_list.h
        classes/my_pair.h
        classes/my_time_scheduler.h
        classes/my_lock_free_stack.h
        classes/test_object.h
)

target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})