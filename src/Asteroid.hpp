#pragma once

#include "Vehicle.hpp"

class Asteroid : public Vehicle
{
public:
    float my_vector;
    float rotation;
    bool IsExist;

    Asteroid();
    void RandPosition(void);
    void Draw(void);
    bool Update(void);
};
