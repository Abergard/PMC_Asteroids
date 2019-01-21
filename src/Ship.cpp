#include "Ship.hpp"

#include <windows.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <cmath>

Ship::Ship()
{
    angle = 0;
    posX = 0;
    posY = 0;
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
            angle = 0;
            posX = 0;
            posY = 0;
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

        posX += cos(angle * M_PI / 180.0f) * speed * delta;
        posY += sin(angle * M_PI / 180.0f) * speed * delta;

        if (posX > 400 || posX < -400)
            posX *= -1;

        if (posY > 300 || posY < -300)
            posY *= -1;
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
    glTranslatef(posX, posY, 0);
    glRotatef(angle, 0, 0, 1);
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
