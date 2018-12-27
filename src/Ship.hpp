#pragma once

#include "Vehicle.hpp"

class Ship : public Vehicle
{
public:
    bool IsDestroy;
    int destroyCount;
    float color;

    int slower;

    Ship(void);
    void Destroy(void);
    void Update(bool ShiftBackMode);
    void Draw(void);
};
