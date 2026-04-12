#pragma once
#include <atomic>

/*
 * EngineState
 *
 * Lifecycle and error state for the application.
 * Separate from AudioData so data structs stay pure data.
 */
struct EngineState {
    std::atomic<bool> is_running { true };

    // Set by any subsystem that hits an unrecoverable error.
    // Main loop checks this and initiates clean shutdown.
    std::atomic<bool> has_error  { false };
};
