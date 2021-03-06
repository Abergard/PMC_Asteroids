#pragma once

#include <chrono>

namespace world
{
class frame_clock
{
public:
    double delta()
    {
        const auto now{std::chrono::high_resolution_clock::now()};
        const std::chrono::duration<float> delta{now - previous_frame};
        previous_frame = now;
        return delta.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> previous_frame{
        std::chrono::high_resolution_clock::now()};
};
}
