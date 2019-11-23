#pragma once

#include "game/game_entity.hpp"
#include "ui/keyboard.hpp"

static void rotate_left(component::transform& racet_local_transform,
                        const float rotation_step)
{
    racet_local_transform.rotation += rotation_step;

    if (racet_local_transform.rotation > 360.0f)
    {
        racet_local_transform.rotation -= 360.0f;
    }
}

static void rotate_right(component::transform& racet_local_transform,
                         const float rotation_step)
{
    if (racet_local_transform.rotation <= 0.0f)
    {
        racet_local_transform.rotation = 360.0f - rotation_step;
    }
    else
    {
        racet_local_transform.rotation -= rotation_step;
    }
}

class keyboard_control_system
{
public:
    void register_entity(game_entity& player)
    {
        entities.push_back(&player);
    }

    void on_key_pressed(const ui::keyboard_key key)
    {
        //FIXME; temporary paste from ship
        const float rotation_step = 2.0f;
        for (auto* player : entities)
        {
            switch (key)
            {
            case ui::keyboard_key::left:
                rotate_left(*player->get<component::transform>(),
                            rotation_step);
                break;
            case ui::keyboard_key::right:
                rotate_right(*player->get<component::transform>(),
                             rotation_step);
                break;

            case ui::keyboard_key::up:
                player->get<component::direction>()->is_forward = true;
                break;
            case ui::keyboard_key::down:
                player->get<component::direction>()->is_forward = false;
                break;
            }
        }
    }

    void on_key_released(const ui::keyboard_key key)
    {
    }

private:
    std::vector<game_entity*> entities;
};
