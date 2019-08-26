#include "actors/Asteroid.hpp"

#include <windows.h>
#include <gl/GLU.h>

#define _USE_MATH_DEFINES
#include <cmath>

Asteroid::Asteroid()
{
    RandPosition();
}

void Asteroid::RandPosition(void)
{
    int site = rand() % 2800;

    if (site < 800)
    {
        transform.location_x = site - 400;
        transform.location_y = 300 + 50;
        transform.rotation = rand() % 180 + 180;
    }
    else if (site < 1400)
    {
        transform.location_x = 400 + 50;
        transform.location_y = site - 1100;
        transform.rotation = rand() % 180 + 90;
    }
    else if (site < 2200)
    {
        transform.location_x = site - 1800;
        transform.location_y = -300 - 50;
        transform.rotation = rand() % 180;
    }
    else
    {
        transform.location_x = -400 - 50;
        transform.location_y = site - 2500;
        transform.rotation = rand() % 180 - 90;
    }

    int speed = base_speed * (rand() % 3 + 1);

    this->my_vector = speed;
}

void Asteroid::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    // only for change
    glTranslatef(transform.location_x, transform.location_y, 0);
    glRotatef(transform.rotation, 0, 0, 1);

    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
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
    transform.location_x += cos(transform.rotation * M_PI / 180.0f) * my_vector * delta;
    transform.location_y += sin(transform.rotation * M_PI / 180.0f) * my_vector * delta;

    if (transform.location_x > 400 + 50 || transform.location_x < -400 - 50)
        return false;

    if (transform.location_y > 300 + 50 || transform.location_y < -300 - 50)
        return false;

    transform.rotation += 1 * delta;

    if (transform.rotation > 360.0f)
        transform.rotation -= 360.0f;

    return true;
}
