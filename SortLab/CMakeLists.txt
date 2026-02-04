project(SortLab)

# Suppress FindBoost deprecation warning (CMP0167)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()
find_package(Boost 1.83 CONFIG REQUIRED)

add_subdirectory(source)
add_subdirectory(tests)