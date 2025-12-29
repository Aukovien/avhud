cmake_minimum_required(VERSION 3.10)
project(avhud)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)


include_directories(${CMAKE_SOURCE_DIR}/external/kissfft)
include_directories(${CMAKE_SOURCE_DIR}/external/miniaudio)

# add imgui at some point
# set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
# file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp ${IMGUI_DIR}/backends/imgui_impl_win32.cpp)

add_executable(avhud
    # main
    src/main.cpp
    src/core/State.h
    src/engine/AudioEngine.cpp
    src/engine/AudioEngine.h

    # 3rd
    external/kissfft/kiss_fft.c
    ${IMGUI_SOURCES}
)

# some are needed for windows
target_link_libraries(avhud d3d11 d3dcompiler dwmapi)
