#pragma once

#include "game/game_entity.hpp"
#include "game/keyboard_control_system.hpp"

struct Ship
{
    game_entity* game_object;

    static const int forward_speed{80};
    static const int backward_speed{-50};
    inline static const float rotation_step = 2.0f;

    float ship_slower{0}; // TODO: what the slower is?

    Ship() = default;
    Ship(std::vector<game_entity>& game_objects,
         component::transform& racket_transform,
         component::direction& racket_direction,
         component::rendering_target& racket_renderer,
         keyboard_control_system& player_controller)
    {
        racket_direction.is_forward = component::direction::forward{true};
        racket_direction.direction_x = 0;
        racket_direction.direction_y = 0;
        racket_renderer.is_visible = true;
        racket_renderer.color.r = 1.0f;
        racket_renderer.color.g = 1.0f;
        racket_renderer.color.b = 1.0f;
        racket_renderer.lines = {
            {+15.0f, 0.0f}, {-15.0f, -10.0f}, {-5.0f, 0.0f}, {-15.0f, +10.0}};
        racket_renderer.points = {{0.0f, 0.0f}};

        game_objects.push_back(
            game_entity{racket_transform, racket_direction, racket_renderer});
        game_object = &game_objects.back();

        player_controller.register_entity(game_objects.back());
    }

    void update(double delta) // override
    {
        auto& local_transform = *game_object->get<component::transform>();

        // if position shipe go ouf of screen should show up on the other side
        // of screen
        if (local_transform.position.x > 400 ||
            local_transform.position.x < -400)
            local_transform.position.x *= -1;

        if (local_transform.position.y > 300 ||
            local_transform.position.y < -300)
            local_transform.position.y *= -1;
    }

    bool play_death_animation(const double delta)
    {

        bool end_of_animation = false;
        ship_slower += delta;
        if (ship_slower > 0.2)
        {
            if (game_object->get<component::rendering_target>()->color.r > 0)
            {
                game_object->get<component::rendering_target>()->color.r -=
                    0.1f;
                game_object->get<component::rendering_target>()->color.g -=
                    0.1f;
                game_object->get<component::rendering_target>()->color.b -=
                    0.1f;
            }
            else
            {
                game_object->get<component::transform>()->rotation = 0;
                game_object->get<component::transform>()->position.x = 0;
                game_object->get<component::transform>()->position.y = 0;
                game_object->get<component::rendering_target>()->color.r =
                    1.0f;
                game_object->get<component::rendering_target>()->color.g =
                    1.0f;
                game_object->get<component::rendering_target>()->color.b =
                    1.0f;
                end_of_animation = true;
            }
            ship_slower = 0;
        }
        return end_of_animation;
    }
};
