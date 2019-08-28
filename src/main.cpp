#include <windows.h>

#include <gl\GLU.h>
#include <stdlib.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>

// #include "actors/Asteroid.hpp"
// #include "actors/Ship.hpp"
#include "windows/Win32Window.hpp"

const WORD ID_TIMER = 1;

float asteroidBuffer = 0;
bool IsAsteroid = false;

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

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrievInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
try
{
    Win32Window window{800, 600};

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
