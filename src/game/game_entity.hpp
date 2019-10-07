#pragma once

#include "components/direction.hpp"
#include "components/rendering_target.hpp"
#include "components/transform.hpp"
#include "world/entity.hpp"

using game_entity = world::entity<component::transform,
                                  component::direction,
                                  component::rendering_target>;
