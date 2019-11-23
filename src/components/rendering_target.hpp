#pragma once

#include <vector>

namespace component
{
struct rendering_target
{
    bool is_visible;

    struct
    {
        float x;
        float y;
    } position;

    float rotation;

    struct
    {
        float r;
        float g;
        float b;
    }color;

    std::vector<std::pair<float, float>> lines;
    std::vector<std::pair<float, float>> points;
};
}
