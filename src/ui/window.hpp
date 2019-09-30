#pragma once

namespace ui
{
class window
{
public:
    virtual ~window() = default;

    virtual void handle_window_events() = 0;
    virtual bool is_open() = 0;
    virtual void swap_buffers() = 0;
};
}
