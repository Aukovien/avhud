cmake_minimum_required(VERSION 3.20)
project(avhud VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# src files
set(SOURCES
    src/main.cpp
    src/app/App.cpp
    src/audio/AudioCapture.cpp
    src/renderer/OverlayWindow.cpp
    src/renderer/Renderer.cpp
)

# WIN32 suppresses the console window in release; keep it for now during dev
add_executable(avhud WIN32 ${SOURCES})

# Include dirs
target_include_directories(avhud PRIVATE
    src/
    external/miniaudio/
    external/kissfft/
)

# Windows system libs
# d2d1        : Direct2D drawing
# dwrite      : DirectWrite (text rendering, needed by D2D)
# dxgi        : DXGI (swap chains / display infrastructure)
# dwmapi      : DWM (window composition / blur behind)
target_link_libraries(avhud PRIVATE
    d2d1
    dwrite
    dxgi
    dwmapi
)

# Compiler warns
if (MSVC)
    target_compile_options(avhud PRIVATE /W4 /WX)
else()
    target_compile_options(avhud PRIVATE -Wall -Wextra -Werror)
endif()
