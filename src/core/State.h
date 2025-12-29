#pragma once
#include <atomic>

/*
 * struct AppState - holds visual data from audio
 * @bass_intensity
 * @treble_intensity
 * @channel_energy
 * @is_running
 */
struct AppState {
    // 0 = silent, 1 = max vol
    std::atomic<float> bass_intensity;
    std::atomic<float> treble_intensity;

    std::atomic<float> channel_energy[8];
    std::atomic<bool> is_running;

    AppState() {
        bass_intensity = 0.0f;
        treble_intensity = 0.0f;
        is_running = true;
        for(int i = 0; i < 8; i++) channel_energy[i] = 0.0f;
    }
};
