#pragma once

#include "components/Transform.hpp"
#include "world_object.hpp"

class Asteroid
{
public:

    void Draw(void);
    bool Update(float delta);

    float current_speed{1};
    bool IsExist{false};

    Transform transform{};
    WorldObject object{&transform};

    static const int base_speed{30};
};
