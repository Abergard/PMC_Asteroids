#pragma once

#define _USE_MATH_DEFINES

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
#include "keyboard.hpp"
#include "windows/Win32Window.hpp"

namespace
{
void draw(const Ship& ship)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    // only for change
    glTranslatef(ship.object.get<Transform>()->location_x,
                 ship.object.get<Transform>()->location_y,
                 0);
    glRotatef(ship.object.get<Transform>()->rotation, 0, 0, 1);
    //

    glBegin(GL_POLYGON);
    glColor3f(ship.color, ship.color, ship.color);
    glVertex2f(+15.0f, 0.0f);
    glVertex2f(-15.0f, -10.0f);
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(-15.0f, +10.0f);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glPopMatrix();
}

void rotate_left(Transform& transform, const float rotation_step)
{
    transform.rotation += rotation_step;

    if (transform.rotation > 360.0f)
    {
        transform.rotation -= 360.0f;
    }
}

void rotate_right(Transform& transform, const float rotation_step)
{
    if (transform.rotation <= 0.0f)
    {
        transform.rotation = 360.0f - rotation_step;
    }
    else
    {
        transform.rotation -= rotation_step;
    }
}

void rotate(const KeyboardKey key,
            Transform& transform,
            const float rotation_step)
{
    if (key == KeyboardKey::left)
    {
        rotate_left(transform, rotation_step);
    }
    if (key == KeyboardKey::right)
    {
        rotate_right(transform, rotation_step);
    }
}
void set_direction(const KeyboardKey key, Direction& direction)
{
    if (key == KeyboardKey::up)
    {
        direction.forward = true;
    }
    else if (key == KeyboardKey::down)
    {
        direction.forward = false;
    }
}
void play_death_animation(Ship& ship, const float delta)
{
    ship.slower += delta;
    if (ship.slower > 0.2)
    {
        ship.is_destroyed = true;
        if (ship.color > 0)
        {
            ship.color -= 0.1f;
        }
        else
        {
            ship.is_destroyed = false;
            ship.object.get<Transform>()->rotation = 0;
            ship.object.get<Transform>()->location_x = 0;
            ship.object.get<Transform>()->location_y = 0;
            ship.deaths = 0;
            ship.color = 1.0f;
        }
        ship.slower = 0;
    }
}

void move_object(WorldObject obj,
                 const int forward_speed,
                 const int backward_speed,
                 const float delta)
{
    const int speed =
        obj.get<Direction>()->forward ? forward_speed : backward_speed;

    obj.get<Transform>()->location_x +=
        cos(obj.get<Transform>()->rotation * M_PI / 180.0f) * speed * delta;
    obj.get<Transform>()->location_y +=
        sin(obj.get<Transform>()->rotation * M_PI / 180.0f) * speed * delta;

    if (obj.get<Transform>()->location_x > 400 ||
        obj.get<Transform>()->location_x < -400)
        obj.get<Transform>()->location_x *= -1;

    if (obj.get<Transform>()->location_y > 300 ||
        obj.get<Transform>()->location_y < -300)
        obj.get<Transform>()->location_y *= -1;
}
}

class Game
{
public:
    Game(Window& w, Keyboard& k) : window{w}, keyboard{k}
    {
        // racket.object.add<Transform>(&racket.transform);
        // racket.object.add<Direction>(&racket.direction);
    }

    void on_pressed(const KeyboardKey key)
    {
        rotate(key, *racket.object.get<Transform>(), rotation_step);
        set_direction(key, *racket.object.get<Direction>());
    }

    void on_released(const KeyboardKey)
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

        draw(racket);

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
        if (keyboard.is(KeyboardKey::space, KeyState::pressed))
        {
            asteroid.RandPosition();
        }

        if (!racket.is_destroyed &&
            is_collided(racket.transform, asteroid.transform))
        {
            racket.is_destroyed = true;
        }

        if (racket.is_destroyed)
        {
            play_death_animation(racket, delta);
        }
        else
        {
            move_object(racket.object,
                        racket.forward_speed,
                        racket.backward_speed,
                        delta);
        }

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

    float asteroidBuffer = 0;
    bool IsAsteroid = false;
    const float rotation_step = 2.0f;
    Ship racket{};
    Asteroid asteroid{};

    Window& window;
    Keyboard& keyboard;
};
