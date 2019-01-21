#pragma once

#include "Vehicle.hpp"

class Asteroid : public Vehicle
{
public:
    Asteroid();
    void RandPosition(void);
    void Draw(void);
    bool Update(float delta);

    float my_vector{1};
    float rotation{0};
    bool IsExist{false};

    const int base_speed{30};
};
