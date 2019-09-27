#pragma once

class Window
{
public:
    virtual ~Window() = default;

    virtual void handle_window_events() = 0;
    virtual bool is_open() = 0;
    virtual void swap_buffers()=0;
};
