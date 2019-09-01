#pragma once

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
#include "frame_clock.hpp"
#include "windows/Win32Window.hpp"

class Game
{
public:
    Game(Window& w) : window{w}
    {
    }

    void run()
    {
        FrameClock frame_clock;

        while (window.is_open())
        {
            // read input
            window.handle_window_events();

            // update game logic
            update_logic(frame_clock.calculate_delta());

            // render surface
            update_surface();
            display_surface(window);
        }
    }

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

private:
    void display_surface(Window& window)
    {
        window.swap_buffers();
    }

    void update_surface()
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

    void update_logic(float delta)
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

    const WORD ID_TIMER = 1;

    float asteroidBuffer = 0;
    bool IsAsteroid = false;
    const float rotation_step = 2.0f;
    bool ShiftBackMode = false;
    Ship racket{};
    Asteroid asteroid{};

    MSG msg;

    Window& window;
};
