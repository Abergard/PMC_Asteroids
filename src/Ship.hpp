#pragma once

#include "Vehicle.hpp"

struct health
{
    int value;
};

class Ship : public Vehicle
{
public:
    Ship(void);
    void Update(float delta, bool ShiftBackMode);
    void Draw(void);

    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    int slower{0};

private:
    void Destroy(float delta);


    const int forward_speed{80};
    const int backward_speed{-50};
    const int max_lifes{6};
};
