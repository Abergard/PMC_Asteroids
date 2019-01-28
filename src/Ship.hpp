#pragma once

#include "Transform.hpp"

struct health
{
    int value;
};

class Ship
{
public:
    Ship(void);
    void Update(float delta, bool ShiftBackMode);
    void Draw(void);

    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    float slower{0};

    Transform transform{};

private:
    void Destroy(float delta);

    const int forward_speed{80};
    const int backward_speed{-50};
    const int max_lifes{6};

};
