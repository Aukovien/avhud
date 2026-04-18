cmake_minimum_required(VERSION 3.20)

# Source Files
set(TEST_SOURCES
    audio/AudioAnalyzerTests.cpp
)

add_executable(avhud_tests ${TEST_SOURCES})

# Include Directories
# doctest  - single header test framework (drop doctest.h into external/doctest/)
# src/     - so tests can include project headers directly
target_include_directories(avhud_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/external/doctest/
    ${CMAKE_SOURCE_DIR}/external/kissfft/
)

# Compiler Warnings
if (MSVC)
    target_compile_options(avhud_tests PRIVATE /W4)
else()
    target_compile_options(avhud_tests PRIVATE -Wall -Wextra)
endif()

# Register with CTest so "ctest" in the build dir runs everything
enable_testing()
add_test(NAME avhud_tests COMMAND avhud_tests)
