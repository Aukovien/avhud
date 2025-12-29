#include <iostream>
#include <thread>
#include <chrono>

// internal
#include "core/State.h"

int main(int argc, char* argv[]) {
    AppState state;

    std::cout << "HUD started" << std::endl;

    while (state.is_running) {

        // eep for 16ms, about 60fps. hardcoding this is bad.
        // for the love of everything include slider and variable later
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
