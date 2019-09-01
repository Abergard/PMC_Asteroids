#include <windows.h>

#include "game.hpp"
#include "windows/Win32Window.hpp"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
try
{
    Win32Window window{800, 600};
    Game game{window};

    window.subscribe([&game](const auto& event) { game.updateGame(event); });
    game.run();
}
catch (const std::exception& ex)
{
    MessageBox(nullptr, ex.what(), "Create Error", MB_ICONEXCLAMATION);
}
catch (...)
{
    MessageBox(
        nullptr, "Unexpected exception", "Create Error", MB_ICONEXCLAMATION);
}
