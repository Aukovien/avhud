#include "audio/AudioCapture.h"
#include <cstdio>

bool AudioCapture::Start(SampleCallback on_samples) {
    callback_ = on_samples;

    // Device Config
    // ma_device_type_loopback captures whatever the system is playing.
    // On Windows this uses WASAPI under the hood, which means there's no extra setup needed.
    // NULL for the device ID means the default playback device's monitor.
    ma_device_config config = ma_device_config_init(ma_device_type_loopback);

    // Loopback mirrors the playback device so we use the playback fields,
    // not capture fields, to describe the format we want.
    config.playback.format   = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate        = 48000;
    config.dataCallback      = DataCallback;
    config.pUserData         = this;

    if (ma_device_init(nullptr, &config, &device_) != MA_SUCCESS) {
        std::fprintf(stderr, "[AudioCapture] Failed to init loopback device.\n"
                             "  Is a playback device active?\n");
        return false;
    }

    if (ma_device_start(&device_) != MA_SUCCESS) {
        std::fprintf(stderr, "[AudioCapture] Failed to start loopback device.\n");
        ma_device_uninit(&device_);
        return false;
    }

    running_ = true;
    return true;
}

void AudioCapture::Stop() {
    if (running_) {
        ma_device_uninit(&device_);
        running_ = false;
    }
}

/*
 * DataCallback
 *
 * Called by miniaudio on a background thread every ~21ms.
 * We just forward the raw samples straight to whoever registered.
 *
 * input:       interleaved f32 samples [L, R, L, R ...]
 * frame_count: number of frames (one frame = one L + one R sample)
 */
void AudioCapture::DataCallback(ma_device* device, void* /*output*/,
                                const void* input, ma_uint32 frame_count) {
    auto* capture = reinterpret_cast<AudioCapture*>(device->pUserData);

    if (capture->callback_) {
        capture->callback_(
            reinterpret_cast<const float*>(input),
            static_cast<int>(frame_count)
        );
    }
}
