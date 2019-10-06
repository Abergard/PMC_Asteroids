#pragma once

#include <tuple>

#include "components/direction.hpp"
#include "components/transform.hpp"
#include "game/game_entity.hpp"
#include "ui/keyboard.hpp"

struct Ship
{
    game_entity game_object;

    static const int forward_speed{80};
    static const int backward_speed{-50};
    inline static const float rotation_step = 2.0f;
};
