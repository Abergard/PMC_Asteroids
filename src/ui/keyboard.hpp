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
    bool is(keyboard_key, key_state);
    bool has(keyboard_key, key_state);
    void set(keyboard_key, key_state);

private:
    bool has_been_changed(keyboard_key);
    frame_key_state& get(keyboard_key);

    std::unordered_map<keyboard_key, frame_key_state> keysFrameStates{};
};
}
