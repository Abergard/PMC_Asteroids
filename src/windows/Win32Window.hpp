#pragma once

#include <functional>
#include <iostream>
#include <windows.h>

#include "actors/Asteroid.hpp"
#include "actors/Ship.hpp"

extern const float rotation_step;
extern bool ShiftBackMode;
extern Ship racket;
extern Asteroid asteroid;

struct Win32Message
{
};

class Win32Window
{
public:
    Win32Window(const std::int32_t window_width,
                const std::int32_t window_height);

    void handle_window_events();
    void subscribe(std::function<void(Win32Message&)>);
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

    HWND window_handle{nullptr};
    HDC hDC{};
    HGLRC hRC{};
    bool opened{true};
};
