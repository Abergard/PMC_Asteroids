#include "Asteroid.hpp"

Asteroid::Asteroid()
{
    IsExist = false;
    this->rotation = 0;
    this->posX = 0;
    this->posY = 0;
    this->angle = 0;
    this->my_vector = vector;
    RandPosition();
}

void Asteroid::RandPosition(void)
{
    int site = rand() % 2800;

    if (site < 800)
    {
        this->posX = site - 400;
        this->posY = 300 + 50;
        this->angle = rand() % 180 + 180;
    }
    else if (site < 1400)
    {
        this->posX = 400 + 50;
        this->posY = site - 1100;
        this->angle = rand() % 180 + 90;
    }
    else if (site < 2200)
    {
        this->posX = site - 1800;
        this->posY = -300 - 50;
        this->angle = rand() % 180;
    }
    else
    {
        this->posX = -400 - 50;
        this->posY = site - 2500;
        this->angle = rand() % 180 - 90;
    }

    int speed = rand() % 3 + 1;

    this->my_vector = vector * speed;
}

void Asteroid::Draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    // only for change
    glTranslatef(posX, posY, 0);
    glRotatef(rotation, 0, 0, 1);

    glBegin(GL_POLYGON);
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

bool Asteroid::Update()
{
    this->posX += cos(angle * M_PI / 180.0f) * my_vector;
    this->posY += sin(angle * M_PI / 180.0f) * my_vector;

    if (this->posX > 400 + 50 || this->posX < -400 - 50)
        return false;

    if (this->posY > 300 + 50 || this->posY < -300 - 50)
        return false;

    rotation += 1;

    if (rotation > 360.0f)
        rotation -= 360.0f;

    return true;
}
