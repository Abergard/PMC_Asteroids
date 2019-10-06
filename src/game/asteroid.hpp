#pragma once

#include "components/transform.hpp"
#include "game/game_entity.hpp"

struct Asteroid
{
    float current_speed{1};
    bool IsExist{false};

    game_entity game_object;

    static const int base_speed{30};
};
