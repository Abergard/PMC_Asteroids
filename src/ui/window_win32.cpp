#include "ui/window_win32.hpp"

#include <gl/GLU.h>

namespace ui
{

static void enable_opengl(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    // get the device context (DC)
    *hDC = GetDC(hWnd);

    // set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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

win32_window::win32_window(const std::int32_t window_width,
                           const std::int32_t window_height)
{
    auto hInstance = GetModuleHandle(nullptr);
    const char* NazwaKlasy = "GL tutorial";

    auto wndProc =
        [](HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
            if (auto* main = reinterpret_cast<win32_window*>(
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

    RECT rect{};

    window_handle = CreateWindowEx(WS_EX_CLIENTEDGE,
                                   NazwaKlasy,
                                   "Zad 2",
                                   WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   window_width,
                                   window_height,
                                   nullptr,
                                   nullptr,
                                   hInstance,
                                   nullptr);

    if (window_handle == nullptr)
    {
        throw std::exception{"Window creation failed"};
    }

    SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)this);

    enable_opengl(window_handle, &hDC, &hRC);
}

void win32_window::read_events()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool win32_window::is_open()
{
    return opened;
}

void win32_window::swap_buffers()
{
    SwapBuffers(hDC);
}

static void send_close_event(HWND hWnd)
{
    PostMessage(hWnd, WM_CLOSE, 0, 0);
}

static void disable_opengl(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}

void win32_window::close_window(HWND hWnd)
{
    opened = false;
    disable_opengl(hWnd, hDC, hRC);
    DestroyWindow(hWnd);
}

void win32_window::subscribe(win32_event_callback callback)
{
    win32_message_callback = callback;
}

LRESULT CALLBACK win32_window::realWndProc(HWND hWnd,
                                           UINT message,
                                           WPARAM wParam,
                                           LPARAM lParam)
{
    win32_message_callback(win32_event{message, wParam});

    switch (message)
    {
    case WM_CLOSE:
        close_window(hWnd);
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:

            send_close_event(hWnd);
            return 0;
        }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
}
