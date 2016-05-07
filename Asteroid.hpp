#pragma once
#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Vehicle.hpp"

class Asteroid : public Vehicle
{
public:
    GLfloat my_vector;
    GLfloat rotation;
    bool IsExist;

    Asteroid();
    void RandPosition(void);
    void Draw(void);
    bool Update(void);
};
