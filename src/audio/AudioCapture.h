#pragma once

#include <functional>
#include "miniaudio.h"

/*
 * AudioCapture
 *
 * Opens a WASAPI loopback device and streams raw interleaved float samples
 * to whoever registered via Start(). Does no analysis itself.
 *
 * Sample format: f32, 2 channels (interleaved L/R), 48000hz
 *
 * The callback fires on a background audio thread — keep it fast and
 * never block inside it.
 */

using SampleCallback = std::function<void(const float* samples, int frame_count)>;

class AudioCapture {
public:
    AudioCapture()  = default;
    ~AudioCapture() = default;

    AudioCapture(const AudioCapture&)            = delete;
    AudioCapture& operator=(const AudioCapture&) = delete;

    // Starts loopback capture. on_samples is called for every buffer.
    // Returns false if miniaudio fails to open the device.
    bool Start(SampleCallback on_samples);

    void Stop();

    bool IsRunning() const { return running_; }

private:
    static void DataCallback(ma_device* device, void* output,
                             const void* input, ma_uint32 frame_count);

    ma_device      device_   = {};
    SampleCallback callback_ = nullptr;
    bool           running_  = false;
};
