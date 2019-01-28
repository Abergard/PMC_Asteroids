#include <windows.h>

#include <gl\GLU.h>
#include <stdlib.h>

#include <ctime>
#include <exception>
#include <iostream>
#include <cmath>
#include <chrono>

#include "Asteroid.hpp"
#include "Ship.hpp"

const float rotation_step = 2.0f;
const WORD ID_TIMER = 1;

bool ShiftBackMode = false;
float asteroidBuffer = 0;
bool IsAsteroid = false;

struct
{
    int x;
    int y;
} window_size = {800, 600};

MSG msg;
HWND g_hWnd;
HDC hDC;
HGLRC hRC;

Ship racket{};
Asteroid asteroid{};

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

void display()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-400, 400, -300, 300, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    racket.Draw();

    if (IsAsteroid == true)
    {
        asteroid.Draw();
    }
}

bool is_collided(const Transform& first,
                 const Transform& second)
{
    const auto x = first.location_x - second.location_x;
    const auto y = first.location_y - second.location_y;
    const auto distance = sqrt((x * x) + (y * y));
    return distance < 54.0f;
}

class Window
{
public:
    Window(HINSTANCE hInstance) : hWnd{create_window(hInstance)}
    {
    }

    ~Window()
    {
        destroy_window();
    }

    HWND handle()
    {
        return hWnd;
    }

    void swap_buffers()
    {
        SwapBuffers(hDC);
    }

private:
    HWND create_window(HINSTANCE hInstance)
    {
        const char* NazwaKlasy = "GL tutorial";

        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WndProc;
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

        enable_opengl(hWnd, &hDC, &hRC);
        return hWnd;
    }

    void destroy_window()
    {
        if (hWnd)
        {
            disable_opengl(hWnd, hDC, hRC);
            DestroyWindow(hWnd);
        }
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

    HWND hWnd;
};

void update(float delta)
{
    if (!racket.is_destroyed)
    {
        if(is_collided(racket.transform, asteroid.transform))
        {
            racket.is_destroyed = true;
        }
    }
    racket.Update(delta, ShiftBackMode);

    if (IsAsteroid)
    {
        IsAsteroid = asteroid.Update(delta);
    }
    else
    {
        if (asteroidBuffer >= 2)
        {
            asteroidBuffer = 0;
            asteroid.RandPosition();
            IsAsteroid = true;
        }
        else
        {
            asteroidBuffer += delta;
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) try
{
    Window window{hInstance};

    // TODO: replace TIMER by DeltaTime
    if (SetTimer(window.handle(), ID_TIMER, 20, nullptr) == 0)
        MessageBox(
            window.handle(), "Can not create timer", "Blad", MB_ICONSTOP);

    srand(time(nullptr));

    bool app_running = true;
    auto previous_frame{std::chrono::high_resolution_clock::now()};
    while (app_running)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            switch (msg.message)
            {
            case WM_QUIT:
                app_running = false;
                break;

            default:
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            const auto now{std::chrono::high_resolution_clock::now()};
            const std::chrono::duration<float> delta{now - previous_frame};
            previous_frame = now;

            display();
            window.swap_buffers();
            update(delta.count());
        }
    }

    return msg.wParam;
}
catch (const std::exception& ex)
{
    MessageBox(nullptr, ex.what(), "Create Error", MB_ICONEXCLAMATION);
}
catch (...)
{
    MessageBox(
        nullptr, "Unexpected exception", "Create Error", MB_ICONEXCLAMATION);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE: // TODO: what to do with it? Is it safe to destroy it in
                   // that way?
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_MENU:
            break;

        case VK_ESCAPE:
            PostQuitMessage(0);
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
    }
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
