#define _USE_MATH_DEFINES

#include "actors/asteroid.hpp"

#include <windows.h>

#include <cmath>
#include <gl/GLU.h>

bool Asteroid::Update(float delta)
{
    game_object.get<transform>()->location_x +=
        static_cast<float>(
            cos(game_object.get<transform>()->rotation * M_PI / 180.0f)) *
        current_speed * delta;
    game_object.get<transform>()->location_y +=
        static_cast<float>(
            sin(game_object.get<transform>()->rotation * M_PI / 180.0f)) *
        current_speed * delta;

    if (game_object.get<transform>()->location_x > 400 + 50 ||
        game_object.get<transform>()->location_x < -400 - 50)
        return false;

    if (game_object.get<transform>()->location_y > 300 + 50 ||
        game_object.get<transform>()->location_y < -300 - 50)
        return false;

    game_object.get<transform>()->rotation += 1 * delta;

    if (game_object.get<transform>()->rotation > 360.0f)
        game_object.get<transform>()->rotation -= 360.0f;

    return true;
}
