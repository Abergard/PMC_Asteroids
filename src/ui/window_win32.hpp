#pragma once

#include <windows.h>

#include <functional>
#include <iostream>

#include "ui/window.hpp"

namespace ui
{

struct win32_event
{
    UINT message;
    WPARAM wparam;
};

class win32_window : public window
{
public:
    using win32_event_callback = std::function<void(win32_event)>;

    win32_window(const std::int32_t window_width,
                 const std::int32_t window_height);

    void handle_window_events() override;
    void subscribe(win32_event_callback);
    bool is_open() override;
    void swap_buffers() override;

private:
    void close_window(HWND hWnd);
    void send_close_event(HWND hWnd);
    LRESULT CALLBACK realWndProc(HWND hWnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam);
    void enable_opengl(HWND hWnd, HDC* hDC, HGLRC* hRC);
    void disable_opengl(HWND hWnd, HDC hDC, HGLRC hRC);

    win32_event_callback win32_message_callback = [](auto) {};

    HWND window_handle{nullptr};
    HDC hDC{};
    HGLRC hRC{};
    bool opened{true};
};
}
