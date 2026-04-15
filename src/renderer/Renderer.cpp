#include "renderer/Renderer.h"
#include <d2d1helper.h>
#include <cmath>

bool Renderer::Init(HWND hwnd, int width, int height) {
    hwnd_   = hwnd;
    width_  = width;
    height_ = height;

    // Create D2D Factory
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        factory_.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    return RecreateTarget();
}

bool Renderer::RecreateTarget() {
    render_target_.Reset();
    brush_.Reset();

    D2D1_RENDER_TARGET_PROPERTIES rtProps =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED   // required for layered windows
            )
        );

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(
            hwnd_,
            D2D1::SizeU(width_, height_),
            D2D1_PRESENT_OPTIONS_IMMEDIATELY   // no vsync :)
        );

    HRESULT hr = factory_->CreateHwndRenderTarget(
        rtProps, hwndProps, render_target_.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    // Create a reusable brush. Colour is changed per draw call
    hr = render_target_->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        brush_.GetAddressOf()
    );
    return SUCCEEDED(hr);
}

void Renderer::Shutdown() {
    brush_.Reset();
    render_target_.Reset();
    factory_.Reset();
}

void Renderer::Draw(const AudioData& audio) {
    if (!render_target_) return;

    render_target_->BeginDraw();

    // Clear to pure black is treated as transparent by LWA_COLORKEY
    render_target_->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

    DrawEdgeBars(audio);
    DrawDebugOverlay(audio);

    HRESULT hr = render_target_->EndDraw();

    // D2DERR_RECREATE_TARGET means the GPU device was lost.
    // Recreate the target and carry on. This is normal on driver updates etc.
    if (hr == D2DERR_RECREATE_TARGET) {
        RecreateTarget();
    }
}

/*
 * DrawEdgeBars
 *
 * Draws a vertical bar on the left and right screen edges whose height
 * scales with bass intensity. Placeholders for now.
 */
void Renderer::DrawEdgeBars(const AudioData& audio) {
    const float bass   = audio.bass_intensity.load();
    const float treble = audio.treble_intensity.load();

    const float bar_width  = 8.0f;
    const float max_height = static_cast<float>(height_) * 0.6f;

    // Left bar (bass)
    float left_h = max_height * bass;
    float left_y = (height_ - left_h) / 2.0f;   // vertically centred

    brush_->SetColor(D2D1::ColorF(0.2f, 0.6f, 1.0f, 0.85f));   // blue
    render_target_->FillRectangle(
        D2D1::RectF(0.0f, left_y, bar_width, left_y + left_h),
        brush_.Get()
    );

    // Right bar (treble)
    float right_h = max_height * treble;
    float right_y = (height_ - right_h) / 2.0f;
    float right_x = static_cast<float>(width_) - bar_width;

    brush_->SetColor(D2D1::ColorF(1.0f, 0.4f, 0.2f, 0.85f));   // orange
    render_target_->FillRectangle(
        D2D1::RectF(right_x, right_y,
                    static_cast<float>(width_), right_y + right_h),
        brush_.Get()
    );
}

/*
 * DrawDebugOverlay
 *
 * Draws small indicator squares for each of the 8 frequency bands along
 * the bottom edge.
 */
void Renderer::DrawDebugOverlay(const AudioData& audio) {
    const float cell  = 24.0f;
    const float gap   = 4.0f;
    const float total = 8 * (cell + gap) - gap;
    float x_start     = (width_ - total) / 2.0f;
    float y_bottom    = static_cast<float>(height_) - 32.0f;

    for (int i = 0; i < 8; i++) {
        float val    = audio.bands[i].load();
        float height = cell * val;   // bar grows upward with band energy

        // Gradient from blue (low) to red (high) bands
        float t = static_cast<float>(i) / 7.0f;
        brush_->SetColor(D2D1::ColorF(t, 0.3f, 1.0f - t, 0.7f));

        float x = x_start + i * (cell + gap);
        render_target_->FillRectangle(
            D2D1::RectF(x, y_bottom - height, x + cell, y_bottom),
            brush_.Get()
        );
    }
}
