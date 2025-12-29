#pragma once

#include <vector>

// int
#include "../core/State.h"

// 3rd
#include "miniaudio.h"
#include "kiss_fft.h"

class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        bool Start(AppState* shared_state);
        void Stop();

    private:
    static void DataCallback(ma_device* pDevice, void* pOutput,
                    const void* pInput, ma_uint32 frameCount);

    ma_device device;
        AppState* state = nullptr; // ptr to shared state

        // kiss fft
        kiss_fft_cfg cfg;
        std::vector<kiss_fft_cpx> fft_in;
        std::vector<kiss_fft_cpx> fft_out;
};
