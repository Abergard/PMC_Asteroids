#define _USE_MATH_DEFINES

#include "actors/asteroid.hpp"

#include <windows.h>

#include <cmath>
#include <gl/GLU.h>

Asteroid::Asteroid(game_entity&& e) : game_object{std::move(e)}
{
    game_object.get<color>()->rgb = 1.0f;
    game_object.get<mesh>()->lines = {{+50.0f, -10.0f},
                                      {+20.0f, -50.0f},
                                      {-5.0f, -50.0f},
                                      {-5.0f, -25.0f},
                                      {-30.0f, -50.0f},
                                      {-50.0f, -10.0f},
                                      {-25.0f, 0.0f},
                                      {-50.0f, +10.0f},
                                      {-15.0f, +45.0f},
                                      {+20.0f, +45.0f}};
}

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
