#include <windows.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <gl\GLU.h>
#include <iostream>
#include <stdlib.h>

#include "actors/Asteroid.hpp"
#include "actors/Ship.hpp"
#include "windows/Win32Window.hpp"

const WORD ID_TIMER = 1;

float asteroidBuffer = 0;
bool IsAsteroid = false;

const float rotation_step = 2.0f;
bool ShiftBackMode = false;
Ship racket{};
Asteroid asteroid{};

MSG msg;

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

bool is_collided(const Transform& first, const Transform& second)
{
    const auto x = first.location_x - second.location_x;
    const auto y = first.location_y - second.location_y;
    const auto distance = sqrt((x * x) + (y * y));
    return distance < 54.0f;
}

void update(float delta)
{
    if (!racket.is_destroyed)
    {
        if (is_collided(racket.transform, asteroid.transform))
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

enum class KeyEvent
{

};

class KeyEventObservator
{
public:
    virtual ~KeyEventObservator() = default;

    void observe(KeyEvent, std::function<void()>);
};

void updateGame(Win32Event event)
{
    switch (event.message)
    {
    case WM_KEYDOWN:
        switch (event.wparam)
        {
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
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
try
{
    Win32Window window{800, 600};
    window.subscribe(updateGame);

    auto previous_frame{std::chrono::high_resolution_clock::now()};
    while (window.is_open())
    {
        window.handle_window_events();

        const auto now{std::chrono::high_resolution_clock::now()};
        const std::chrono::duration<float> delta{now - previous_frame};
        previous_frame = now;

        update(delta.count());
        display();
        window.swap_buffers();
    }
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
