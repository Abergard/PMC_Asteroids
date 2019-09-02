#pragma once

#include <tuple>

#include "components/Direction.hpp"
#include "components/Transform.hpp"
#include "keyboard.hpp"

struct health
{
    int value;
};

template <typename... Components>
class TWorldObject
{
public:
    template <typename Component>
    void add(Component* c)
    {
        std::get<Component*>(components) = c;
    }

    template <typename Component>
    Component* get()
    {
        return const_cast<Component*>(
            static_cast<const TWorldObject<Components...>&>(*this)
                .get<Component>());
    }

    template <typename Component>
    const Component* get() const
    {
        return std::get<Component*>(components);
    }

private:
    std::tuple<Components*...> components;
};

using WorldObject = TWorldObject<Transform, Direction>;

struct Ship
{
    bool is_destroyed{false};
    int deaths{0};
    float color{1.0f};
    float slower{0};
    Transform transform;
    Direction direction{Direction::Forward{true}};
    WorldObject ship;

    const int forward_speed{80};
    const int backward_speed{-50};
    const int max_lifes{6};
    const float rotation_step = 2.0f;
};
