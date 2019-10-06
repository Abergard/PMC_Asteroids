#pragma once

#include <vector>

namespace component
{
struct mesh
{
    std::vector<std::pair<float, float>> lines;
    std::vector<std::pair<float, float>> points;
};
}
