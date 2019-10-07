#pragma once

#include <vector>

namespace component
{
struct rendering_target
{
    bool is_visible;
    float location_x;
    float location_y;
    float rotation;
    float rgb;
    std::vector<std::pair<float, float>> lines;
    std::vector<std::pair<float, float>> points;
};
}
