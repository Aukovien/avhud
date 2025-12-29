#include <cmath>

#define MINIAUDIO_IMPLEMENTATION
#include "AudioEngine.h"

const int FFT_SIZE = 1024;

AudioEngine::AudioEngine() {
    // init the fft plan
    // 0 = Forward FFT (Time -> Freq)
    cfg = kiss_fft_alloc(FFT_SIZE, 0, nullptr, nullptr);

    // resize vectors to hold data
    fft_in.resize(FFT_SIZE);
    fft_out.resize(FFT_SIZE);
}

AudioEngine::~AudioEngine() {
    free(cfg);
}

/*
 * AudioEngine()
 * @pDevice
 * @pOutput
 * @pInput
 * @frameCount
 *
 * runs every 21ms on background thread
 */
void AudioEngine::DataCallback(ma_device* pDevice, void* pOutput,
                        const void* pInput, ma_uint32 frameCount) {

    AudioEngine* engine = (AudioEngine*)pDevice->pUserData;
    float* pSampleData = (float*)pInput;

    if (frameCount > FFT_SIZE) frameCount = FFT_SIZE;

    for (int i = 0; i < frameCount; i++) {
        // input is interleaved: [L, R, L, R...]
        // we grab every 2nd sample (left only for now)
        float sample = pSampleData[i * 2];

        // hanning window (smooths the signal to reduce noise)
        float multiplier = 0.5f * (1.0f - cosf(2.0f * 3.14159f * i / (FFT_SIZE - 1)));

        engine->fft_in[i].r = sample * multiplier;
        engine->fft_in[i].i = 0.0f;
    }

    for (int i = frameCount; i < FFT_SIZE; i++) {
        engine->fft_in[i].r = 0.0f;
        engine->fft_in[i].i = 0.0f;
    }

    kiss_fft(engine->cfg, engine->fft_in.data(), engine->fft_out.data());

    // analyze freq
    float bass_energy = 0.0f;
    float treble_energy = 0.0f;

    // bass: 20hz - 150hz; 1-4
    for (int i = 1; i < 4; i++) {
        float magnitude = sqrtf(engine->fft_out[i].r * engine->fft_out[i].r +
                                engine->fft_out[i].i * engine->fft_out[i].i);
        bass_energy += magnitude;
    }

    // treble: 2000hz - 4000hz 40-80
    for (int i = 40; i < 80; i++) {
        float magnitude = sqrtf(engine->fft_out[i].r * engine->fft_out[i].r +
                                engine->fft_out[i].i * engine->fft_out[i].i);
        treble_energy += magnitude;
    }

    // upd state
    // scale down because FFT values can be huge
    if (engine->state) {
        engine->state->bass_intensity.store(bass_energy / 50.0f);
        engine->state->treble_intensity.store(treble_energy / 50.0f);
    }
}

bool AudioEngine::Start(AppState* shared_state) {
    this->state = shared_state;

    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_f32;
    config.capture.channels = 2;
    config.sampleRate = 48000;
    config.periodSizeInFrames = FFT_SIZE;
    config.dataCallback = DataCallback;
    config.pUserData = this;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) return false;
    return (ma_device_start(&device) == MA_SUCCESS);
}

void AudioEngine::Stop() {
    ma_device_uninit(&device);
}
