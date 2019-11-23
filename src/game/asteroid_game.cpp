#define _USE_MATH_DEFINES

#include "game/asteroid_game.hpp"

#include <windows.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <gl/GLU.h>
#include <iostream>
#include <stdlib.h>

#include "world/frame_clock.hpp"

template <typename... Args>
static void reserve_memory(const std::size_t size, Args&... args)
{
    (args.reserve(size), ...);
}

asteroid_game::asteroid_game(ui::window& w, ui::keyboard& k)
    : window{w}, keyboard{k}
{
    reserve_memory(
        number_of_game_objects, transforms, directions, game_objects);

    racket = Ship(game_objects,
                  transforms.emplace_back(),
                  directions.emplace_back(),
                  game_rendering_pipeline.create_rendering_target(),
                  player_controller);
    asteroid = Asteroid(game_objects,
                        transforms.emplace_back(),
                        directions.emplace_back(),
                        game_rendering_pipeline.create_rendering_target());
}

void asteroid_game::on_pressed(const ui::keyboard_key key)
{
    switch (key)
    {
    case ui::keyboard_key::left:
    case ui::keyboard_key::right:
    case ui::keyboard_key::up:
    case ui::keyboard_key::down:
        player_controller.on_key_pressed(key);
        break;

        // ...
    case ui::keyboard_key::space:
        asteroid.move_to_new_position();
        break;
    case ui::keyboard_key::unknown:
        break;
    }
}

void asteroid_game::on_released(const ui::keyboard_key)
{
}

void asteroid_game::run()
{
    world::frame_clock clock{};

    while (window.is_open())
    {
        window.read_events();
        update_game_logic(clock.delta());
        update_rendering_target_components();
        game_rendering_pipeline.render();
        window.swap_buffers();
    }
}

void asteroid_game::update_rendering_target_components()
{
    for (auto& game_object : game_objects)
    {
        const auto* position = game_object.get<component::transform>();
        auto* renderer = game_object.get<component::rendering_target>();

        if (position && renderer)
        {
            renderer->position.x = position->position.x;
            renderer->position.y = position->position.y;
            renderer->rotation = position->rotation;
        }
    }
}

static void move_object(component::transform& local_transform,
                        component::direction& object_direction,
                        const int move_speed,
                        const int rotation_speed,
                        const double delta)
{
    local_transform.position.x +=
        object_direction.direction_x * move_speed * delta;

    local_transform.position.y +=
        object_direction.direction_y * move_speed * delta;

    local_transform.rotation += rotation_speed * delta;

    if (local_transform.rotation > 360.0f)
        local_transform.rotation -= 360.0f;
}

// TODO: move to collision system
static bool is_collided(const component::transform& first,
                        const component::transform& second)
{
    const auto x = first.position.x - second.position.x;
    const auto y = first.position.y - second.position.y;
    const auto distance = sqrt((x * x) + (y * y));
    return distance < 54.0f;
}

void asteroid_game::update_game_logic(double delta)
{
    if (!ship_destroyed &&
        is_collided(*racket.game_object->get<component::transform>(),
                    *asteroid.game_object->get<component::transform>()))
    {
        ship_destroyed = true;
    }

    int racket_move_speed = 0;
    int racket_rotation_speed = 0;

    // health system
    if (ship_destroyed)
    {
        bool animation_end = racket.play_death_animation(delta);
        if (animation_end)
        {
            ship_destroyed = false;
            ship_deaths = 0;
        }
    }
    else
    {
        racket_move_speed =
            racket.game_object->get<component::direction>()->is_forward ?
                Ship::forward_speed :
                Ship::backward_speed;
    }

    auto& racket_transform = *racket.game_object->get<component::transform>();
    auto& racket_direction = *racket.game_object->get<component::direction>();

    racket_direction.direction_x =
        cos(racket_transform.rotation * M_PI / 180.0f);
    racket_direction.direction_y =
        sin(racket_transform.rotation * M_PI / 180.0f);

    // movement system
    move_object(racket_transform,
                racket_direction,
                racket_move_speed,
                racket_rotation_speed,
                delta);

    auto& asteroid_transform =
        *asteroid.game_object->get<component::transform>();

    auto rotation = asteroid_transform.rotation;
    int asteroid_rotation_speed = 100;

    if (rotation < 0)
        asteroid_rotation_speed *= -1;

    move_object(asteroid_transform,
                *asteroid.game_object->get<component::direction>(),
                asteroid.current_speed,
                asteroid_rotation_speed,
                delta);

    // level logic
    racket.update(delta);
    asteroid.update(delta);
}
