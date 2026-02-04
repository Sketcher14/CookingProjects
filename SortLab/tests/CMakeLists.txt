project(SortLab_Tests)

# Unit tests executable (Google Test)
add_executable(SortLab_UnitTests
        algo/sort_tests.cpp
)
target_link_libraries(SortLab_UnitTests PRIVATE
        SortLab_Algo
        Utility_Test
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(SortLab_UnitTests)


# Performance benchmarks executable (Google Benchmark)
add_executable(SortLab_Benchmark
        algo/sort_perf_tests.cpp
)
target_link_libraries(SortLab_Benchmark PRIVATE
        SortLab_Algo
        Utility_Test
        benchmark::benchmark
)
