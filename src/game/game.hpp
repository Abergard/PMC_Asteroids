#pragma once

#define _USE_MATH_DEFINES

#include <windows.h>

#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <gl/GLU.h>
#include <iostream>
#include <stdlib.h>

#include "game/asteroid.hpp"
#include "game/ship.hpp"
#include "ui/keyboard.hpp"
#include "ui/window_win32.hpp"
#include "world/frame_clock.hpp"

namespace
{
void rand_asteroid_properties(Asteroid& asteroid)
{
    int site = rand() % 2800;

    auto& t = *asteroid.game_object.get<transform>();

    if (site < 800)
    {
        t.location_x = static_cast<float>(site - 400);
        t.location_y = 300 + 50;
        t.rotation = static_cast<float>(rand() % 180 + 180);
    }
    else if (site < 1400)
    {
        t.location_x = 400 + 50;
        t.location_y = static_cast<float>(site - 1100);
        t.rotation = static_cast<float>(rand() % 180 + 90);
    }
    else if (site < 2200)
    {
        t.location_x = static_cast<float>(site - 1800);
        t.location_y = -300 - 50;
        t.rotation = static_cast<float>(rand() % 180);
    }
    else
    {
        t.location_x = -400 - 50;
        t.location_y = static_cast<float>(site - 2500);
        t.rotation = static_cast<float>(rand() % 180 - 90);
    }
    asteroid.current_speed =
        static_cast<float>(Asteroid::base_speed * (rand() % 3 + 1));
}

void draw(const game_entity& game_object)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    if (const auto* local_transform = game_object.get<transform>())
    {
        glTranslatef(
            local_transform->location_x, local_transform->location_y, 0);
        glRotatef(local_transform->rotation, 0, 0, 1);
    }

    if (const auto* object_color = game_object.get<color>())
    {
        glColor3f(object_color->rgb, object_color->rgb, object_color->rgb);
    }

    if (const auto* object_mesh = game_object.get<mesh>())
    {
        glBegin(GL_POLYGON);
        for (const auto& point : object_mesh->lines)
        {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
        glEnd();
    }
    glPopMatrix();
}

void rotate_left(transform& transform, const float rotation_step)
{
    transform.rotation += rotation_step;

    if (transform.rotation > 360.0f)
    {
        transform.rotation -= 360.0f;
    }
}

void rotate_right(transform& transform, const float rotation_step)
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

void play_death_animation(Ship& ship, const float delta)
{
    ship.slower += delta;
    if (ship.slower > 0.2)
    {
        ship.is_destroyed = true;
        if (ship.game_object.get<color>()->rgb > 0)
        {
            ship.game_object.get<color>()->rgb -= 0.1f;
        }
        else
        {
            ship.is_destroyed = false;
            ship.game_object.get<transform>()->rotation = 0;
            ship.game_object.get<transform>()->location_x = 0;
            ship.game_object.get<transform>()->location_y = 0;
            ship.deaths = 0;
            ship.game_object.get<color>()->rgb = 1.0f;
        }
        ship.slower = 0;
    }
}

void move_object(transform& local_transform,
                 const int move_speed,
                 const int rotation_speed,
                 const float delta)
{
    local_transform.location_x +=
        cos(local_transform.rotation * M_PI / 180.0f) * move_speed * delta;
    local_transform.location_y +=
        sin(local_transform.rotation * M_PI / 180.0f) * move_speed * delta;

    local_transform.rotation += rotation_speed * delta;

    if (local_transform.rotation > 360.0f)
        local_transform.rotation -= 360.0f;
}
}

class Game
{
public:
    Game(ui::window& w, ui::keyboard& k) : window{w}, keyboard{k}
    {
        rand_asteroid_properties(asteroid);
    }

    void on_pressed(const ui::keyboard_key key)
    {
        switch (key)
        {
        case ui::keyboard_key::left:
            rotate_left(*racket.game_object.get<transform>(),
                        Ship::rotation_step);
            break;
        case ui::keyboard_key::right:
            rotate_right(*racket.game_object.get<transform>(),
                         Ship::rotation_step);
            break;
        case ui::keyboard_key::up:
            racket.game_object.get<direction>()->forward = true;
            break;
        case ui::keyboard_key::down:
            racket.game_object.get<direction>()->forward = false;
            break;
        case ui::keyboard_key::space:
            rand_asteroid_properties(asteroid);
            break;
        case ui::keyboard_key::unknown:
            break;
        }
    }

    void on_released(const ui::keyboard_key)
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
    void display_surface(ui::window& window)
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

        draw(racket.game_object);

        if (asteroid_visible)
        {
            draw(asteroid.game_object);
        }
    }

    bool is_collided(const transform& first, const transform& second)
    {
        const auto x = first.location_x - second.location_x;
        const auto y = first.location_y - second.location_y;
        const auto distance = sqrt((x * x) + (y * y));
        return distance < 54.0f;
    }

    bool is_asteroid_visible()
    {
        if (!asteroid_visible ||
            asteroid.game_object.get<transform>()->location_x > 400 + 50 ||
            asteroid.game_object.get<transform>()->location_x < -400 - 50 ||
            asteroid.game_object.get<transform>()->location_y > 300 + 50 ||
            asteroid.game_object.get<transform>()->location_y < -300 - 50)
        {
            return false;
        }
        return true;
    }

    void update_logic(float delta)
    {
        if (!racket.is_destroyed &&
            is_collided(*racket.game_object.get<transform>(),
                        *asteroid.game_object.get<transform>()))
        {
            racket.is_destroyed = true;
        }

        if (racket.is_destroyed)
        {
            play_death_animation(racket, delta);
        }
        else
        {
            const int move_speed =
                racket.game_object.get<direction>()->forward ?
                    Ship::forward_speed :
                    Ship::backward_speed;
            auto rotation_speed = 0;

            auto& local_transform = *racket.game_object.get<transform>();
            move_object(local_transform, move_speed, rotation_speed, delta);

            if (local_transform.location_x > 400 ||
                local_transform.location_x < -400)
                local_transform.location_x *= -1;

            if (local_transform.location_y > 300 ||
                local_transform.location_y < -300)
                local_transform.location_y *= -1;
        }

        if (asteroid_visible = is_asteroid_visible())
        {
            move_object(*asteroid.game_object.get<transform>(),
                        asteroid.current_speed,
                        1,
                        delta);
        }
        else
        {
            if (asteroidBuffer >= 2)
            {
                asteroidBuffer = 0;
                rand_asteroid_properties(asteroid);
                asteroid_visible = true;
            }
            else
            {
                asteroidBuffer += delta;
            }
        }
    }

    Ship create_racket()
    {
        Ship racket{};

        racket.game_object =
            game_entity{transforms[0], directions[0], colors[0], meshs[0]};

        racket.game_object.get<direction>()->forward =
            direction::Forward{true};
        racket.game_object.get<color>()->rgb = 1.0f;
        racket.game_object.get<mesh>()->lines = {
            {+15.0f, 0.0f}, {-15.0f, -10.0f}, {-5.0f, 0.0f}, {-15.0f, +10.0}};

        // racket.game_object.visible = true;

        return racket;
    }

    Asteroid create_asteroid()
    {
        Asteroid asteroid{};
        asteroid.game_object = game_entity{transforms[1], colors[1], meshs[1]};
        asteroid.game_object.get<color>()->rgb = 1.0f;
        asteroid.game_object.get<mesh>()->lines = {{+50.0f, -10.0f},
                                                   {+20.0f, -50.0f},
                                                   {-5.0f, -50.0f},
                                                   {-5.0f, -25.0f},
                                                   {-30.0f, -50.0f},
                                                   {-50.0f, -10.0f},
                                                   {-25.0f, 0.0f},
                                                   {-50.0f, +10.0f},
                                                   {-15.0f, +45.0f},
                                                   {+20.0f, +45.0f}};

        // asteroid.game_object.visible = false;

        return asteroid;
    }

    float asteroidBuffer = 0;
    std::array<transform, 2> transforms{};
    std::array<direction, 1> directions{};
    std::array<color, 2> colors{};
    std::array<mesh, 2> meshs{};

    Ship racket{create_racket()};
    Asteroid asteroid{create_asteroid()};

    // bool is_asteroid_visible{false};
    bool asteroid_visible{false};

    ui::window& window;
    ui::keyboard& keyboard;
};
