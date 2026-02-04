cmake_minimum_required(VERSION 4.0 FATAL_ERROR)
project(CookingProjects)

set(CMAKE_CXX_STANDARD 23)
if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wconversion)
endif ()

# Adding sanitizers (GCC/Clang only)
if (NOT MSVC)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
    #add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
    #add_link_options(-fsanitize=thread)
endif ()


# Making available common dependencies GoogleTest, GoogleBenchmark for all subprojects
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

# Disable the character-conversion warning for GoogleTest target (Clang only)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gtest PRIVATE -Wno-character-conversion)
endif ()


# Enable testing for CTest
enable_testing()

# My subprojects
add_subdirectory(Utility)

add_subdirectory(MultithreadingLab)
add_subdirectory(Sandbox)
add_subdirectory(SortLab)