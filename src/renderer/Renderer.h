#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d2d1.h>
#include <wrl/client.h>

#include "core/AudioData.h"

/*
 * Renderer
 *
 * Owns the Direct2D render target bound to the overlay HWND.
 * Each call to Draw() produces one frame.
 *
 * Direct2D is retained-mode in terms of resource management but
 * immediate-mode in terms of draw calls - BeginDraw/EndDraw bracket
 * every frame, which maps naturally onto a game-loop structure.
 *
 * Layers to be added as the project grows:
 *   - Edge bars (bass-reactive)
 *   - Screen-centre wave (treble-reactive)
 *   - Band spectrum along bottom edge
 */
class Renderer {
public:
    Renderer()  = default;
    ~Renderer() = default;

    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool Init(HWND hwnd, int width, int height);

    void Shutdown();

    void Draw(const AudioData& audio);

private:
    bool RecreateTarget();

    // Draw helpers (one per visual element)
    void DrawEdgeBars(const AudioData& audio);
    void DrawDebugOverlay(const AudioData& audio);

    // D2D resources
    Microsoft::WRL::ComPtr<ID2D1Factory>          factory_;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> render_target_;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  brush_;

    HWND hwnd_   = nullptr;
    int  width_  = 0;
    int  height_ = 0;
};
