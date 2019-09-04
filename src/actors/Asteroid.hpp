#pragma once

#include "components/Transform.hpp"
#include "entity.hpp"

class Asteroid
{
public:
    Asteroid(entity&&);

    void Draw(void);
    bool Update(float delta);

    float current_speed{1};
    bool IsExist{false};

    entity game_object;

    static const int base_speed{30};
};
