#include "app/App.h"
#include <chrono>
#include <thread>

bool App::Init() {
    if (!window_.Create(L"avhud")) return false;

    if (!renderer_.Init(window_.GetHwnd(),
                        window_.GetWidth(),
                        window_.GetHeight())) {
        return false;
    }

    // AudioCapture and AudioAnalyzer will be initialized here

    return true;
}

void App::Run() {
    using clock    = std::chrono::steady_clock;
    using ms       = std::chrono::duration<float, std::milli>;

    // Target ~60fps. Will be replaced by proper timing later
    const float target_ms = 1000.0f / 60.0f;

    while (engine_state_.is_running) {
        auto frame_start = clock::now();

        // OS msgs
        if (!window_.PumpMessages()) {
            engine_state_.is_running = false;
            break;
        }

        renderer_.Draw(audio_data_);

        // Frame cap
        float elapsed = ms(clock::now() - frame_start).count();
        if (elapsed < target_ms) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(
                    static_cast<int>(target_ms - elapsed)));
        }
    }
}

void App::Shutdown() {
    // Shutdown in reverse init order
    renderer_.Shutdown();
    window_.Destroy();
}
