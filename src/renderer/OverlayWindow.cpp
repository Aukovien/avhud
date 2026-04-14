#include "renderer/OverlayWindow.h"
#include <dwmapi.h>
#include <stdexcept>

bool OverlayWindow::Create(const std::wstring& title) {
    hinstance_ = GetModuleHandle(nullptr);

    // Register window class
    WNDCLASSEXW wc   = {};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hinstance_;
    wc.lpszClassName = kClassName;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    // Fully transparent background — Direct2D owns all drawing
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

    if (!RegisterClassExW(&wc)) return false;

    // Resolve full monitor dimensions
    // Use the primary monitor for now.
    // TODO: support multi-monitor selection.
    width_  = GetSystemMetrics(SM_CXSCREEN);
    height_ = GetSystemMetrics(SM_CYSCREEN);

    // Extended style flags
    // WS_EX_LAYERED    : required for transparency (SetLayeredWindowAttributes)
    // WS_EX_TRANSPARENT: hit-testing passes through to windows below
    // WS_EX_TOPMOST    : always above normal windows
    // WS_EX_NOACTIVATE : clicking the overlay does not steal focus from game
    DWORD exStyle = WS_EX_LAYERED    |
                    WS_EX_TRANSPARENT |
                    WS_EX_TOPMOST     |
                    WS_EX_NOACTIVATE;

    // WS_POPUP : no title bar, no borders
    hwnd_ = CreateWindowExW(
        exStyle,
        kClassName,
        title.c_str(),
        WS_POPUP,
        0, 0, width_, height_,
        nullptr, nullptr,
        hinstance_,
        this                // pass 'this' so WndProc can retrieve it
    );

    if (!hwnd_) return false;

    // Transparency setup
    // LWA_COLORKEY: treat pure black (0x00000000) as transparent.
    // Direct2D will clear to black each frame; only drawn pixels are visible.
    SetLayeredWindowAttributes(hwnd_, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Disable DWM rounded corners (Windows 11)
    // Prevents the OS from rounding overlay corners and clipping content.
    DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_DONOTROUND;
    DwmSetWindowAttribute(hwnd_, DWMWA_WINDOW_CORNER_PREFERENCE,
                          &corner, sizeof(corner));

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);

    return true;
}

void OverlayWindow::Destroy() {
    if (hwnd_) {
        DestroyWindow(hwnd_);
        hwnd_ = nullptr;
    }
    UnregisterClassW(kClassName, hinstance_);
}

bool OverlayWindow::PumpMessages() {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

LRESULT CALLBACK OverlayWindow::WndProc(HWND hwnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam) {
    // On creation, stash the OverlayWindow* in the window's user data slot
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hwnd, GWLP_USERDATA,
                         reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
    }

    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // Allow Alt+F4 to close the overlay during development
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE || wParam == VK_F4) {
                PostQuitMessage(0);
                return 0;
            }
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
