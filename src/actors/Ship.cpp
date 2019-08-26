#include "actors/Ship.hpp"

#include <windows.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <cmath>

Ship::Ship()
{
}

void Ship::Destroy(float delta)
{
    slower += delta;
    if (slower > 0.2)
    {
        is_destroyed = true;
        if (color > 0)
        {
            color -= 0.1f;
        }
        else
        {
            is_destroyed = false;
            transform.rotation = 0;
            transform.location_x = 0;
            transform.location_y = 0;
            deaths = 0;
            color = 1.0f;
        }
        slower = 0;
    }
}

void Ship::Update(float delta, bool ShiftBackMode)
{
    if (is_destroyed == false)
    {
        const int speed = ShiftBackMode ? backward_speed : forward_speed;

        transform.location_x += cos(transform.rotation * M_PI / 180.0f) * speed * delta;
        transform.location_y += sin(transform.rotation * M_PI / 180.0f) * speed * delta;

        if (transform.location_x > 400 || transform.location_x < -400)
            transform.location_x *= -1;

        if (transform.location_y > 300 || transform.location_y < -300)
            transform.location_y *= -1;
    }
    else
    {
        Destroy(delta);
    }
}

void Ship::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    // only for change
    glTranslatef(transform.location_x, transform.location_y, 0);
    glRotatef(transform.rotation, 0, 0, 1);
    //

    glBegin(GL_POLYGON);
    glColor3f(color, color, color);
    glVertex2f(+15.0f, 0.0f);
    glVertex2f(-15.0f, -10.0f);
    glVertex2f(-5.0f, 0.0f);
    glVertex2f(-15.0f, +10.0f);
    glEnd();

    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glPopMatrix();
}
