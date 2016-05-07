#pragma once
#include <windows.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include "Vehicle.hpp"

class Ship : public Vehicle
{
public:
    bool IsDestroy;
    int destroyCount;
    GLfloat color;

    int slower;

    Ship(void);
    void Destroy(void);
    void Update(bool ShiftBackMode);
    void Draw(void);
};
