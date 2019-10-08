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

static void rand_asteroid_properties(Asteroid& asteroid)
{
    int site = rand() % 2800;

    auto& t = *asteroid.game_object->get<component::transform>();

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

static Asteroid create_asteroid(std::vector<game_entity>& game_objects,
                                component::transform& asteroid_transform,
                                component::rendering_target& asteroid_renderer)
{
    asteroid_renderer.is_visible = true;
    asteroid_renderer.rgb = 1.0f;
    asteroid_renderer.lines = {{+50.0f, -10.0f},
                               {+20.0f, -50.0f},
                               {-5.0f, -50.0f},
                               {-5.0f, -25.0f},
                               {-30.0f, -50.0f},
                               {-50.0f, -10.0f},
                               {-25.0f, 0.0f},
                               {-50.0f, +10.0f},
                               {-15.0f, +45.0f},
                               {+20.0f, +45.0f}};

    asteroid_renderer.points = {{0.0f, 0.0f}};

    Asteroid asteroid{};
    game_objects.push_back(game_entity{asteroid_transform, asteroid_renderer});
    asteroid.game_object = &game_objects.back();

    return asteroid;
}

static Ship create_racket(std::vector<game_entity>& game_objects,
                          component::transform& racket_transform,
                          component::direction& racket_direction,
                          component::rendering_target& racket_renderer,
                          keyboard_control_system& player_controller)
{
    racket_direction.is_forward = component::direction::forward{true};
    racket_renderer.is_visible = true;
    racket_renderer.rgb = 1.0f;
    racket_renderer.lines = {
        {+15.0f, 0.0f}, {-15.0f, -10.0f}, {-5.0f, 0.0f}, {-15.0f, +10.0}};
    racket_renderer.points = {{0.0f, 0.0f}};

    Ship ship{};
    game_objects.push_back(
        game_entity{racket_transform, racket_direction, racket_renderer});
    ship.game_object = &game_objects.back();

    player_controller.register_entity(game_objects.back());

    return ship;
}

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

    racket = create_racket(game_objects,
                           transforms.emplace_back(),
                           directions.emplace_back(),
                           game_rendering_pipeline.create_rendering_target(),
                           player_controller);
    asteroid =
        create_asteroid(game_objects,
                        transforms.emplace_back(),
                        game_rendering_pipeline.create_rendering_target());
    rand_asteroid_properties(asteroid);
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
        rand_asteroid_properties(asteroid);
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
            renderer->location_x = position->location_x;
            renderer->location_y = position->location_y;
            renderer->rotation = position->rotation;
        }
    }
}

static bool is_object_inside_screen(const game_entity& game_object)
{
    if (game_object.get<component::transform>()->location_x > 400 + 50 ||
        game_object.get<component::transform>()->location_x < -400 - 50 ||
        game_object.get<component::transform>()->location_y > 300 + 50 ||
        game_object.get<component::transform>()->location_y < -300 - 50)
    {
        return false;
    }
    return true;
}

static void move_object(component::transform& racet_local_transform,
                        const int move_speed,
                        const int rotation_speed,
                        const double delta)
{
    racet_local_transform.location_x +=
        cos(racet_local_transform.rotation * M_PI / 180.0f) * move_speed *
        delta;
    racet_local_transform.location_y +=
        sin(racet_local_transform.rotation * M_PI / 180.0f) * move_speed *
        delta;

    racet_local_transform.rotation += rotation_speed * delta;

    if (racet_local_transform.rotation > 360.0f)
        racet_local_transform.rotation -= 360.0f;
}

void asteroid_game::play_death_animation(Ship& ship, const double delta)
{
    ship_slower += delta;
    if (ship_slower > 0.2)
    {
        ship_destroyed = true;
        if (ship.game_object->get<component::rendering_target>()->rgb > 0)
        {
            ship.game_object->get<component::rendering_target>()->rgb -= 0.1f;
        }
        else
        {
            ship_destroyed = false;
            ship.game_object->get<component::transform>()->rotation = 0;
            ship.game_object->get<component::transform>()->location_x = 0;
            ship.game_object->get<component::transform>()->location_y = 0;
            ship.game_object->get<component::rendering_target>()->rgb = 1.0f;
            ship_deaths = 0;
        }
        ship_slower = 0;
    }
}

// TODO: move to collision system
static bool is_collided(const component::transform& first,
                        const component::transform& second)
{
    const auto x = first.location_x - second.location_x;
    const auto y = first.location_y - second.location_y;
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
    int asteroid_rotation_speed = 0;

    // health system
    if (ship_destroyed)
    {
        play_death_animation(racket, delta);
    }
    else
    {
        racket_move_speed =
            racket.game_object->get<component::direction>()->is_forward ?
                Ship::forward_speed :
                Ship::backward_speed;
    }

    // movement system
    move_object(*racket.game_object->get<component::transform>(),
                racket_move_speed,
                racket_rotation_speed,
                delta);

    move_object(*asteroid.game_object->get<component::transform>(),
                asteroid.current_speed,
                asteroid_rotation_speed,
                delta);

    // level logic
    auto& racet_local_transform =
        *racket.game_object->get<component::transform>();

    if (racet_local_transform.location_x > 400 ||
        racet_local_transform.location_x < -400)
        racet_local_transform.location_x *= -1;

    if (racet_local_transform.location_y > 300 ||
        racet_local_transform.location_y < -300)
        racet_local_transform.location_y *= -1;

    if (!is_object_inside_screen(*asteroid.game_object))
    {
        asteroidBuffer += delta;

        if (asteroidBuffer >= 2)
        {
            asteroidBuffer = 0;
            rand_asteroid_properties(asteroid);
        }
    }
}
