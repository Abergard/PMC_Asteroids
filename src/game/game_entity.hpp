#pragma once

#include "components/color.hpp"
#include "components/direction.hpp"
#include "components/mesh.hpp"
#include "components/transform.hpp"
#include "world/entity.hpp"

struct game_entity : entity<transform, direction, color, mesh>
{
};
