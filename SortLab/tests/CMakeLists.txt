project(SortLab_Tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Disable the character-conversion warning for GoogleTest targets
target_compile_options(gtest PRIVATE -Wno-character-conversion)

add_executable(SortLab_Tests
        algo/sort_test.cpp
)

target_link_libraries(SortLab_Tests PRIVATE GTest::gtest_main)
target_link_libraries(SortLab_Tests PRIVATE SortLab_Algo)

include(GoogleTest)
gtest_discover_tests(SortLab_Tests)
