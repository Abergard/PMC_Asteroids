#pragma once

#include <windows.h>

#include <functional>
#include <iostream>

struct Win32Event
{
    UINT message;
    WPARAM wparam;
};

class Win32Window
{
public:
    using Win32EventCallback = std::function<void(Win32Event)>;

    Win32Window(const std::int32_t window_width,
                const std::int32_t window_height);

    void handle_window_events();
    void subscribe(Win32EventCallback);
    bool is_open();
    void swap_buffers();

private:
    void close_window(HWND hWnd);
    void send_close_event(HWND hWnd);
    LRESULT CALLBACK realWndProc(HWND hWnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam);
    void enable_opengl(HWND hWnd, HDC* hDC, HGLRC* hRC);
    void disable_opengl(HWND hWnd, HDC hDC, HGLRC hRC);

    Win32EventCallback win32_message_callback = [](auto) {};

    HWND window_handle{nullptr};
    HDC hDC{};
    HGLRC hRC{};
    bool opened{true};
};
