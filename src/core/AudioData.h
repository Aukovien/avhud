#pragma once
#include <atomic>

/*
 * AudioData
 *
 * All fields are atomic so no mutex is needed for single-writer,
 * single-reader access across threads.
 *
 * Value ranges are normalised to [0.0, 1.0] — the analyzer is
 * responsible for scaling before storing here.
 */
struct AudioData {
    std::atomic<float> bass_intensity   { 0.0f };
    std::atomic<float> mid_intensity    { 0.0f };
    std::atomic<float> treble_intensity { 0.0f };

    // 8 equal-width frequency bands, low → high
    std::atomic<float> bands[8] {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
};
