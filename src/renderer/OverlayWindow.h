#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

/*
 * OverlayWindow
 *
 * Creates a fullscreen, transparent, always-on-top, click-through Win32
 * window suitable for rendering a game HUD overlay.
 *
 * Properties:
 *   - WS_EX_LAYERED    : enables per-pixel alpha / colour-key transparency
 *   - WS_EX_TRANSPARENT: mouse/keyboard events fall through to the game
 *   - WS_EX_TOPMOST    : always rendered above other windows
 *   - No title bar / borders (WS_POPUP)
 *
 * The window registers its own WNDCLASS and runs a Win32 message pump
 * via PumpMessages(), which should be called once per frame.
 */
class OverlayWindow {
public:
    OverlayWindow()  = default;
    ~OverlayWindow() = default;

    // Non-copyable
    OverlayWindow(const OverlayWindow&)            = delete;
    OverlayWindow& operator=(const OverlayWindow&) = delete;

    // Creates the window. Returns false on failure.
    bool Create(const std::wstring& title = L"avhud");

    // Destroys the window and unregisters the class.
    void Destroy();

    // Drains the Win32 message queue for this window.
    // Returns false when WM_QUIT has been received (time to shut down).
    bool PumpMessages();

    HWND GetHwnd() const { return hwnd_; }
    int  GetWidth()  const { return width_; }
    int  GetHeight() const { return height_; }

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
                                    WPARAM wParam, LPARAM lParam);

    HWND      hwnd_      = nullptr;
    HINSTANCE hinstance_ = nullptr;
    int       width_     = 0;
    int       height_    = 0;

    static constexpr wchar_t kClassName[] = L"avhud_overlay";
};
