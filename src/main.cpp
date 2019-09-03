#include <windows.h>

#include "game.hpp"
#include "keyboard.hpp"
#include "windows/Win32Window.hpp"

KeyState get_key_state(const int wm_key_state)
{
    if (wm_key_state == WM_KEYDOWN)
        return KeyState::pressed;

    if (wm_key_state == WM_KEYUP)
        return KeyState::unpressed;

    return KeyState::none;
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
            const auto key_state = get_key_state(event.message);

            if (key_state != KeyState::none)
                keyboard.update(key, key_state);

            if (key_state == KeyState::pressed)
            {
                game.on_pressed(key);
            }
            else if (key_state == KeyState::pressed)
            {
                game.on_released(key);
            }
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
