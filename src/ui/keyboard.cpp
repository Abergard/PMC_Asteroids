#include "ui/keyboard.hpp"

namespace ui
{
bool keyboard::is(keyboard_key key, key_state state)
{
    return get(key).state == state;
}

bool keyboard::has(keyboard_key key, key_state state)
{
    return has_been_changed(key) && is(key, state);
}

void keyboard::set(keyboard_key key, key_state state)
{
    // if (key == keyboard_key::space)
    //     MessageBox(nullptr,
    //                "Unexpected exception in keyboard",
    //                "Create Error in keyboard",
    //                MB_ICONEXCLAMATION);

    get(key).changed_in_frame = (get(key).state != state);
    get(key).state = state;
}

bool keyboard::has_been_changed(keyboard_key key)
{
    return get(key).changed_in_frame;
}

frame_key_state& keyboard::get(keyboard_key key)
{
    return keysFrameStates[key];
}
}
