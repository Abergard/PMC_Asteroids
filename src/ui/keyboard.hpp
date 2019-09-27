#pragma once

#include <unordered_map>

enum class KeyboardKey
{
    unknown = -1,
    left,
    up,
    right,
    down,
    space
};

enum class KeyState
{
    none = -1,
    unpressed,
    pressed
};

struct FrameKeyState
{
    KeyState state;
    bool changed_in_frame;
};

class Keyboard
{
public:
    bool is(KeyboardKey key, KeyState state)
    {
        return get(key).state == state;
    }

    bool has(KeyboardKey key, KeyState state)
    {
        return has_been_changed(key) && is(key, state);
    }

    void update(KeyboardKey key, KeyState state)
    {
        // if (key == KeyboardKey::space)
        //     MessageBox(nullptr,
        //                "Unexpected exception in keyboard",
        //                "Create Error in keyboard",
        //                MB_ICONEXCLAMATION);

        get(key).changed_in_frame = (get(key).state != state);
        get(key).state = state;
    }

private:
    bool has_been_changed(KeyboardKey key)
    {
        return get(key).changed_in_frame;
    }

    FrameKeyState& get(KeyboardKey key)
    {
        return keyboard[key];
    }

    std::unordered_map<KeyboardKey, FrameKeyState> keyboard{};
};
