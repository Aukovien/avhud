#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "app/App.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    App app;

    if (!app.Init()) return -1;

    app.Run();
    app.Shutdown();

    return 0;
}
