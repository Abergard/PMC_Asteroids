#define _USE_MATH_DEFINES

#include "game/asteroid_game.hpp"
#include "ui/keyboard.hpp"
#include "ui/window_win32.hpp"

ui::key_state get_key_state(const int wm_key_state)
{
    if (wm_key_state == WM_KEYDOWN)
        return ui::key_state::pressed;

    if (wm_key_state == WM_KEYUP)
        return ui::key_state::unpressed;

    return ui::key_state::none;
}

ui::keyboard_key to_keyboard_key(const int wm_key)
{
    switch (wm_key)
    {
    case VK_LEFT:
        return ui::keyboard_key::left;
    case VK_RIGHT:
        return ui::keyboard_key::right;
    case VK_UP:
        return ui::keyboard_key::up;
    case VK_DOWN:
        return ui::keyboard_key::down;
    case VK_SPACE:
        return ui::keyboard_key::space;
    default:
        return ui::keyboard_key::unknown;
    }
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
try
{
    ui::keyboard keyboard;
    ui::win32_window window{800, 600};
    asteroid_game game{window, keyboard};

    window.subscribe([&game, &keyboard](const auto& event) {
        const auto key{to_keyboard_key(event.wparam)};
        const auto state = get_key_state(event.message);

        if (key != ui::keyboard_key::unknown && state != ui::key_state::none)
        {
            keyboard.set(key, state);

            if (state == ui::key_state::pressed)
            {
                game.on_pressed(key);
            }
            else if (state == ui::key_state::pressed)
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
