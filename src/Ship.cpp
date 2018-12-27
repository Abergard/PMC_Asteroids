#include "Ship.hpp"

#include <windows.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <cmath>

Ship::Ship()
{
    this->slower = 0;
    this->angle = 0;
    this->vector = 2;
    this->posX = 0;
    this->posY = 0;
    this->destroyCount = 0;
    this->IsDestroy = false;
    this->color = 1.0f;
}

void Ship::Destroy()
{
    if (slower == 5)
    {
        this->IsDestroy = true;
        if (this->color > 0)
            this->color -= 0.2f;

        this->destroyCount = (this->destroyCount + 1) % 6;

        if (this->destroyCount == 0)
        {
            this->IsDestroy = false;
            this->angle = 0;
            this->posX = 0;
            this->posY = 0;
            this->destroyCount = 0;
            this->color = 1.0f;
        }
    }
    slower = (slower + 1) % 6;
}

void Ship::Update(bool ShiftBackMode)
{
    if (IsDestroy == false)
    {
        GLfloat MODE = 1.0f;

        if (ShiftBackMode)
            MODE = -1.0f;

        this->posX += MODE * cos(angle * M_PI / 180.0f) * vector;
        this->posY += MODE * sin(angle * M_PI / 180.0f) * vector;

        if (this->posX > 400 || this->posX < -400)
            this->posX *= -1;

        if (this->posY > 300 || this->posY < -300)
            this->posY *= -1;
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
    glColor3f(this->color, this->color, this->color);
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
