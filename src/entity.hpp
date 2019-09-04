#pragma once

#include <tuple>

#include "components/Direction.hpp"
#include "components/Transform.hpp"

namespace detail
{
template <typename... Components>
class entity
{
public:
    template <typename... Args>
    explicit entity(Args&... args)
    {
        ((this->add(&args)), ...);
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
            static_cast<const entity<Components...>&>(*this).get<Component>());
    }

    template <typename Component>
    const Component* get() const
    {
        return std::get<Component*>(components);
    }

private:
    std::tuple<Components*...> components;
};
}
using entity = detail::entity<Transform, Direction>;
