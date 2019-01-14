#pragma once

#include "Vehicle.hpp"

class Ship : public Vehicle
{
public:
    Ship(void);
    void Destroy(void);
    void Update(bool ShiftBackMode);
    void Draw(void);

    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    int slower{0};
};
