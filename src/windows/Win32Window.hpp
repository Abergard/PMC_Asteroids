#pragma once

#include <iostream>
#include <windows.h>

#include "actors/Asteroid.hpp"
#include "actors/Ship.hpp"

const float rotation_step = 2.0f;
bool ShiftBackMode = false;

struct
{
    int x;
    int y;
} window_size = {800, 600};

HWND g_hWnd;
HDC hDC;
HGLRC hRC;

Ship racket{};
Asteroid asteroid{};

class Win32Window
{
public:
    Win32Window()
    {
        create_window();
    }

    void handle_window_events()
    {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool is_open()
    {
        return opened;
    }

    void swap_buffers()
    {
        SwapBuffers(hDC);
    }

private:
    void close_window(HWND hWnd, HDC hDC, HGLRC hRC)
    {
        opened = false;
        disable_opengl(hWnd, hDC, hRC);
    }

    void close(HWND hWnd)
    {
        PostMessage(hWnd, WM_CLOSE, 0, 0);
    }

    LRESULT CALLBACK realWndProc(HWND hWnd,
                                 UINT message,
                                 WPARAM wParam,
                                 LPARAM lParam)
    {
        switch (message)
        {
        case WM_CLOSE:
            close_window(hWnd, hDC, hRC);
            break;

        case WM_KEYDOWN:
            switch (wParam)
            {
            case VK_MENU:
                break;

            case VK_ESCAPE:
                close(hWnd);
                break;

            case VK_LEFT:
                racket.transform.rotation += rotation_step;

                if (racket.transform.rotation > 360.0f)
                    racket.transform.rotation -= 360.0f;
                break;

            case VK_RIGHT:
                if (racket.transform.rotation <= 0.0f)
                    racket.transform.rotation = 360.0f - rotation_step;
                else
                    racket.transform.rotation -= rotation_step;
                break;

            case VK_UP:
                ShiftBackMode = false;
                break;

            case VK_DOWN:
                ShiftBackMode = true;
                break;

            case VK_SPACE:
                asteroid.RandPosition();
                break;
            }
            return 0;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    void create_window()
    {
        auto hInstance = GetModuleHandle(nullptr);
        const char* NazwaKlasy = "GL tutorial";

        auto wndProc =
            [](HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
                if (auto* main = reinterpret_cast<Win32Window*>(
                        GetWindowLongPtr(window, GWLP_USERDATA)))
                {
                    return main->realWndProc(window, message, wParam, lParam);
                }
                return DefWindowProc(window, message, wParam, lParam);
            };

        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = wndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = NazwaKlasy;
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

        if (!RegisterClassEx(&wc))
        {
            throw std::exception("Window registration failed");
        }

        auto hWnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                                   NazwaKlasy,
                                   "Zad 2",
                                   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   window_size.x,
                                   window_size.y,
                                   nullptr,
                                   nullptr,
                                   hInstance,
                                   nullptr);

        if (hWnd == nullptr)
        {
            throw std::exception{"Window creation failed"};
        }

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

        enable_opengl(hWnd, &hDC, &hRC);
    }

    void enable_opengl(HWND hWnd, HDC* hDC, HGLRC* hRC)
    {
        PIXELFORMATDESCRIPTOR pfd;
        int format;

        // get the device context (DC)
        *hDC = GetDC(hWnd);

        // set the pixel format for the DC
        ZeroMemory(&pfd, sizeof(pfd));

        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;

        format = ChoosePixelFormat(*hDC, &pfd);
        SetPixelFormat(*hDC, format, &pfd);

        // create and enable the render context (RC)
        *hRC = wglCreateContext(*hDC);
        wglMakeCurrent(*hDC, *hRC);

        glPointSize(2.0f);
    }

    void disable_opengl(HWND hWnd, HDC hDC, HGLRC hRC)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(hRC);
        ReleaseDC(hWnd, hDC);
    }

    bool opened{true};
};
