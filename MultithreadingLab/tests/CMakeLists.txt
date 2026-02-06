project(MultithreadingLab_Tests)

# Unit tests executable (GoogleTest)
add_executable(MultithreadingLab_UnitTests
        thread_pool_tests.cpp
        timer_manager_tests.cpp
)

target_link_libraries(MultithreadingLab_UnitTests PRIVATE
        MultithreadingLab_Library
        Utility_Test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(MultithreadingLab_UnitTests)

