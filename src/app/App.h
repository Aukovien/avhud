#pragma once
#include "core/AudioData.h"
#include "core/EngineState.h"
#include "audio/AudioCapture.h"
#include "renderer/OverlayWindow.h"
#include "renderer/Renderer.h"

/*
 * App
 *
 * Owns all subsystems and runs the main loop.
 * main.cpp constructs App and calls Run(). Nothing else tho.
 *
 * Subsystems added in later steps:
 *   - AudioAnalyzer (Step 3)
 */
class App {
public:
    App()  = default;
    ~App() = default;

    App(const App&)            = delete;
    App& operator=(const App&) = delete;

    // Returns false if any subsystem fails to initialise.
    bool Init();

    // Runs until the overlay window is closed or ESC is pressed.
    void Run();

    // Tears down all subsystems in reverse init order.
    void Shutdown();

private:
    EngineState    engine_state_;
    AudioData      audio_data_;
    AudioCapture   capture_;
    OverlayWindow  window_;
    Renderer       renderer_;
};
