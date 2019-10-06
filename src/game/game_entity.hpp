#pragma once

#include "components/color.hpp"
#include "components/direction.hpp"
#include "components/mesh.hpp"
#include "components/transform.hpp"
#include "world/entity.hpp"

using game_entity = world::entity<component::transform,
                                  component::direction,
                                  component::color,
                                  component::mesh>;
