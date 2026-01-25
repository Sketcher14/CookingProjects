project(SortLab_Main)

# Creating a library with algorithms
add_library(SortLab_Algo INTERFACE
        algo/sort.inl
        algo/sort.h
)
target_include_directories(SortLab_Algo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SortLab_Algo INTERFACE Boost::boost)


# Adding an executable for this target
add_executable(SortLab_Main main.cpp)
target_link_libraries(SortLab_Main PRIVATE SortLab_Algo)

