#define _USE_MATH_DEFINES

#include "actors/Asteroid.hpp"

#include <windows.h>

#include <cmath>
#include <gl/GLU.h>

Asteroid::Asteroid(entity&& e) : game_object{std::move(e)}
{
}

void Asteroid::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    // only for change
    glTranslatef(game_object.get<transform>()->location_x,
                 game_object.get<transform>()->location_y,
                 0);
    glRotatef(game_object.get<transform>()->rotation, 0, 0, 1);

    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glVertex2f(+50.0f, -10.0f);
    glVertex2f(+20.0f, -50.0f);
    glVertex2f(-5.0f, -50.0f);
    glVertex2f(-5.0f, -25.0f);
    glVertex2f(-30.0f, -50.0f);
    glVertex2f(-50.0f, -10.0f);
    glVertex2f(-25.0f, 0.0f);
    glVertex2f(-50.0f, +10.0f);
    glVertex2f(-15.0f, +45.0f);
    glVertex2f(+20.0f, +45.0f);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
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
