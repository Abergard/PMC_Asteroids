#pragma once

#include <tuple>

template <typename... Components>
class entity
{
public:
    template <typename... Args>
    entity(Args&... args)
    {
        ((this->set(&args)), ...);
    }

    template <typename Component>
    void set(Component* c)
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
