#pragma once

#include <tuple>

#include "components/Direction.hpp"
#include "components/Transform.hpp"
#include "keyboard.hpp"
#include "world_object.hpp"

struct health
{
    int value;
};

struct Ship
{
    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    float slower{0};
    Transform transform;
    Direction direction{Direction::Forward{true}};
    WorldObject object{&transform, &direction};

    const int forward_speed{80};
    const int backward_speed{-50};
    const int max_lifes{6};
    const float rotation_step = 2.0f;
};
