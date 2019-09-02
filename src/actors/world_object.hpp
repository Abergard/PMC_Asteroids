#pragma once

#include <tuple>

#include "components/Transform.hpp"
#include "components/Direction.hpp"

template <typename... Components>
class TWorldObject
{
public:
    template <typename... Args>
    explicit TWorldObject(Args... args)
    {
        ((this->add(args)), ...);
    }
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
