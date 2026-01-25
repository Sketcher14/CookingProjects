project(SortLab_Tests)

# Unit tests executable (Google Test)
add_executable(SortLab_UnitTest
        algo/sort_test.cpp
)
target_link_libraries(SortLab_UnitTest PRIVATE
        SortLab_Algo
        Utility_Test
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(SortLab_UnitTest)


# Performance benchmarks executable (Google Benchmark)
add_executable(SortLab_Benchmark
        algo/sort_perf_test.cpp
)
target_link_libraries(SortLab_Benchmark PRIVATE
        SortLab_Algo
        Utility_Test
        benchmark::benchmark
)
