#pragma once

#include <tuple>

#include "components/Direction.hpp"
#include "components/Transform.hpp"
#include "keyboard.hpp"
#include "world_object.hpp"

class Ship
{
public:
    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    float slower{0};
    Transform transform;
    Direction direction{Direction::Forward{true}};
    WorldObject object{&transform, &direction};

    static const int forward_speed{80};
    static const int backward_speed{-50};
    inline static const float rotation_step = 2.0f;
};
