#include "Ship.hpp"

#include <windows.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <cmath>

namespace
{
constexpr int ship_life{6};
}

Ship::Ship()
{
    angle = 0;
    base_speed = 2;
    posX = 0;
    posY = 0;
}

void Ship::Destroy()
{
    if (slower == 5)
    {
        is_destroyed = true;
        if (color > 0)
            color -= 0.2f;

        deaths = (deaths + 1) % ship_life;

        if (deaths == 0)
        {
            is_destroyed = false;
            angle = 0;
            posX = 0;
            posY = 0;
            deaths = 0;
            color = 1.0f;
        }
    }
    slower = (slower + 1) % 6;
}

void Ship::Update(bool ShiftBackMode)
{
    if (is_destroyed == false)
    {
        GLfloat MODE = 1.0f;

        if (ShiftBackMode)
        {
            MODE = -1.0f;
            base_speed = 1;
        }
        else
        {
            base_speed = 2;
        }

        posX += MODE * cos(angle * M_PI / 180.0f) * base_speed;
        posY += MODE * sin(angle * M_PI / 180.0f) * base_speed;

        if (posX > 400 || posX < -400)
            posX *= -1;

        if (posY > 300 || posY < -300)
            posY *= -1;
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
