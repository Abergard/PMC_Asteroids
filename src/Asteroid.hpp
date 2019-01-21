#pragma once

#include "Vehicle.hpp"

class Asteroid : public Vehicle
{
public:
    Asteroid();
    void RandPosition(void);
    void Draw(void);
    bool Update(float delta);

    float my_vector;
    float rotation;
    bool IsExist;
};
