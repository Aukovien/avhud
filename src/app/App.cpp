#include "app/App.h"
#include <chrono>
#include <thread>

bool App::Init() {
    // Overlay window
    if (!window_.Create(L"avhud")) return false;

    // Renderer
    if (!renderer_.Init(window_.GetHwnd(),
                        window_.GetWidth(),
                        window_.GetHeight())) {
        return false;
    }

    // Audio capture
    // Callback fires on the audio thread. Currently just a stub that
    // confirms samples are arriving. AudioAnalyzer hooks in here at Step 3.
    if (!capture_.Start([](const float* /*samples*/, int /*frame_count*/) {
        // samples are arriving. nothing to do until AudioAnalyzer exists
    })) {
        return false;
    }

    return true;
}

void App::Run() {
    using clock    = std::chrono::steady_clock;
    using ms       = std::chrono::duration<float, std::milli>;

    // Target ~60fps. Will be replaced by proper timing in a later step.
    const float target_ms = 1000.0f / 60.0f;

    while (engine_state_.is_running) {
        auto frame_start = clock::now();

        // Process OS messages
        if (!window_.PumpMessages()) {
            engine_state_.is_running = false;
            break;
        }

        // Render
        // audio_data_ is all zeros until AudioAnalyzer is wired in (Step 3).
        // The overlay will open and the debug band squares will be flat.
        // That's expected and confirms the overlay is working correctly.
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
    capture_.Stop();
    renderer_.Shutdown();
    window_.Destroy();
}
