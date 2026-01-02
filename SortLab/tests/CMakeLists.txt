project(SortLab_Tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723 # 1.17.0
)
FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG eddb0241389718a23a42db6af5f0164b6e0139af # v1.9.4
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(
        googletest
        googlebenchmark
)

# Disable the character-conversion warning for GoogleTest targets
target_compile_options(gtest PRIVATE -Wno-character-conversion)

# Unit tests executable (GoogleTest)
add_executable(SortLab_UnitTest
        algo/sort_test.cpp
)

target_link_libraries(SortLab_UnitTest PRIVATE GTest::gtest_main)
target_link_libraries(SortLab_UnitTest PRIVATE SortLab_Algo)

include(GoogleTest)
gtest_discover_tests(SortLab_UnitTest)

# Performance benchmarks executable (Google Benchmark)
add_executable(SortLab_Benchmark
        algo/sort_perf_test.cpp
)

target_link_libraries(SortLab_Benchmark PRIVATE benchmark::benchmark)
target_link_libraries(SortLab_Benchmark PRIVATE SortLab_Algo)
