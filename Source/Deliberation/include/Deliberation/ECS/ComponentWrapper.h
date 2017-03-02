#pragma once

#include <Deliberation/ECS/Component.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

template<typename T>
class ComponentWrapper final:
    public Component<ComponentWrapper<T>>
{
public:
    template<typename ... Args>
    ComponentWrapper(Args &&... args);

    T & value();
    const T & value() const;

private:
    T m_value;
};

}

#include <Deliberation/ECS/ComponentWrapper.inl>