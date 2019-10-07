#pragma once

#include "components/rendering_target.hpp"

namespace graphics
{
class opengl_old_pipeline
{
public:
    opengl_old_pipeline(const std::size_t number_of_objects);

    void render();
    component::rendering_target& create_rendering_target();

private:
    std::vector<component::rendering_target> rendering_targets{};
};
}
