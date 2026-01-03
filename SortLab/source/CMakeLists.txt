project(SortLab_Main)

add_executable(SortLab_Main main.cpp)

add_library(SortLab_Algo INTERFACE
        algo/sort.inl
        algo/sort.h
)

# 'PUBLIC' means both 'Algo' and anything that links to it gets this include path
target_include_directories(SortLab_Algo INTERFACE .)
target_link_libraries(SortLab_Algo INTERFACE Boost::boost)

target_link_libraries(SortLab_Main PRIVATE SortLab_Algo)
target_link_libraries(SortLab_Main PUBLIC Utility)

