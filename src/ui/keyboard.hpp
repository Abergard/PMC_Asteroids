#pragma once

#include <unordered_map>

namespace ui
{
enum class keyboard_key
{
    unknown = -1,
    left,
    up,
    right,
    down,
    space
};

enum class key_state
{
    none = -1,
    unpressed,
    pressed
};

struct frame_key_state
{
    key_state state;
    bool changed_in_frame;
};

class keyboard
{
public:
    bool is(keyboard_key key, key_state state)
    {
        return get(key).state == state;
    }

    bool has(keyboard_key key, key_state state)
    {
        return has_been_changed(key) && is(key, state);
    }

    void set(keyboard_key key, key_state state)
    {
        // if (key == keyboard_key::space)
        //     MessageBox(nullptr,
        //                "Unexpected exception in keyboard",
        //                "Create Error in keyboard",
        //                MB_ICONEXCLAMATION);

        get(key).changed_in_frame = (get(key).state != state);
        get(key).state = state;
    }

private:
    bool has_been_changed(keyboard_key key)
    {
        return get(key).changed_in_frame;
    }

    frame_key_state& get(keyboard_key key)
    {
        return keysFrameStates[key];
    }

    std::unordered_map<keyboard_key, frame_key_state> keysFrameStates{};
};
}
