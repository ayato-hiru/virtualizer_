cmake_minimum_required(VERSION 3.14)

# set the project name
project(
    virtualizer_
    VERSION 0.1.0
    DESCRIPTION "Video making software designed for programmers"
)

# include test and pack
include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(src/common)
add_subdirectory(src/data_generation)
add_subdirectory(src/plugin_compiler)
add_subdirectory(app)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(test)
endif()