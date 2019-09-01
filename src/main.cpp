#include <windows.h>

#include "game.hpp"
#include "keyboard.hpp"
#include "windows/Win32Window.hpp"

KeyState get_key_state(const int wm_key_state)
{
    if (wm_key_state == WM_KEYDOWN)
        return KeyState::pressed;

    return KeyState::unpressed;
}

KeyboardKey to_keyboard_key(const int wm_key)
{
    switch (wm_key)
    {
    case VK_LEFT:
        return KeyboardKey::left;
    case VK_RIGHT:
        return KeyboardKey::right;
    case VK_UP:
        return KeyboardKey::up;
    case VK_DOWN:
        return KeyboardKey::down;
    case VK_SPACE:
        return KeyboardKey::space;
    default:
        return KeyboardKey::unknown;
    }
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
try
{
    Keyboard keyboard;
    Win32Window window{800, 600};
    Game game{window, keyboard};

    window.subscribe([&game, &keyboard](const auto& event) {
        if (const auto key{to_keyboard_key(event.wparam)};
            key != KeyboardKey::unknown)
        {
            keyboard.update(key, get_key_state(event.message));
        }
    });
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
