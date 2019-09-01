#pragma once

#include <windows.h>

#include <functional>
#include <iostream>

#include "windows/window.hpp"

struct Win32Event
{
    UINT message;
    WPARAM wparam;
};

enum class KeyboardKey
{
    Unknown,
    Space,
    ArrowLeft,
    ArrowUp,
    ArrowRight,
    ArrowDown
};

class Keyboard
{
public:
    virtual ~Keyboard() = default;

    virtual bool is_key_up(KeyboardKey) const = 0;
    virtual bool is_key_down(KeyboardKey) const = 0;
};

class Win32Window : public Window
{
public:
    using Win32EventCallback = std::function<void(Win32Event)>;

    Win32Window(const std::int32_t window_width,
                const std::int32_t window_height);

    void handle_window_events() override;
    void subscribe(Win32EventCallback);
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

    Win32EventCallback win32_message_callback = [](auto) {};

    HWND window_handle{nullptr};
    HDC hDC{};
    HGLRC hRC{};
    bool opened{true};
};
