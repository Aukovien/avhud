#include <iostream>
#include <thread>
#include <chrono>

// internal
#include "core/State.h"
#include "engine/AudioEngine.h"
// wrapper for window creation
// renderer/imgui

int main(int argc, char* argv[]) {
    AppState state;

    AudioEngine audio;
    if (!audio.Start(&state)) {
        std::cerr << "Failed to start audio capture. Is a device available?" << std::endl;
        return -1;
    }

    std::cout << "HUD started" << std::endl;

    while (state.is_running) {

        // eep for 16ms, about 60fps. hardcoding this is bad.
        // for the love of everything include slider and variable later
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    audio.Stop();

    return 0;
}
