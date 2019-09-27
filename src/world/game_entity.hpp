#pragma once

#include "world/entity.hpp"
#include "components/direction.hpp"
#include "components/transform.hpp"
#include "components/color.hpp"

using game_entity = entity<transform, direction, color>;
