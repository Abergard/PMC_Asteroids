#pragma once

#include "components/Transform.hpp"
#include "world/game_entity.hpp"

struct Asteroid
{
    bool Update(float delta);

    float current_speed{1};
    bool IsExist{false};

    game_entity game_object;

    static const int base_speed{30};
};
