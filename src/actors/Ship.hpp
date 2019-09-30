#pragma once

#include <tuple>

#include "components/direction.hpp"
#include "components/transform.hpp"
#include "game_entity.hpp"
#include "ui/keyboard.hpp"

class Ship
{
public:
    Ship(game_entity&& e) : game_object{std::move(e)}
    {
        game_object.get<direction>()->forward = direction::Forward{true};
        game_object.get<color>()->rgb = 1.0f;
        game_object.get<mesh>()->lines = {
            {+15.0f, 0.0f}, {-15.0f, -10.0f}, {-5.0f, 0.0f}, {-15.0f, +10.0}};
    }

    bool is_destroyed{false};
    int deaths{0};
    float slower{0};

    game_entity game_object;

    static const int forward_speed{80};
    static const int backward_speed{-50};
    inline static const float rotation_step = 2.0f;
};
