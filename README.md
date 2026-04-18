# avhud

A real-time audio-reactive HUD overlay for Windows. Visualizes system audio as on-screen elements that sit transparently over any application.

## Features

- Loopback capture: Reacts to any audio playing on the system, no per-app setup
- Transparent, click-through fullscreen overlay
- Direct2D renderingL: Lightweight, no GPU overhead
- FFT-based frequency analysis (bass, mid, treble, 8-band)

## Requirements

- Windows 10 or later
- CMake 3.20+
- Visual Studio 2022 (MSVC)

## Building

```bash
git clone --recurse-submodules https://github.com/Aukovien/avhud.git
cd avhud
cmake -B build -S . -G "Visual Studio 17 2022"
cmake --build build
```

## External Dependencies

| Library | Purpose |
|---|---|
| [miniaudio](https://github.com/mackron/miniaudio) | Audio capture via WASAPI loopback |
| [kissfft](https://github.com/mborgerding/kissfft) | FFT frequency analysis |

## Running Tests

```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
src/
├── app/        - application lifecycle
├── audio/      - capture and analysis
├── core/       - shared data structs
└── renderer/   - overlay window and Direct2D
```

## License

BSD 3-Clause
