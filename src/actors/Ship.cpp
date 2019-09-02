#define _USE_MATH_DEFINES

#include "actors/Ship.hpp"

#include <windows.h>

#include <cmath>
#include <gl/GLU.h>

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

void Ship::on_update(float delta)
{
    if (is_destroyed == false)
    {
        const int speed = direction.forward ? forward_speed : backward_speed;

        transform.location_x +=
            cos(transform.rotation * M_PI / 180.0f) * speed * delta;
        transform.location_y +=
            sin(transform.rotation * M_PI / 180.0f) * speed * delta;

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
