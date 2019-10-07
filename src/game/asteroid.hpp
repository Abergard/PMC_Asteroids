#pragma once

#include "game/game_entity.hpp"

struct Asteroid
{
    game_entity* game_object;

    // TODO: replace by movement component
    float current_speed{1};
    static const int base_speed{30};
    // ...
};
