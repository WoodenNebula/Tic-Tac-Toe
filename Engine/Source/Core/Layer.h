#pragma once

#include "Events/Event.h"
#include <string>

namespace Engine
{
class Layer
{
public:
    Layer(std::string_view Name = "Layer") : m_Name(Name) {}
    virtual ~Layer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnEvent(Events::EventBase& event) {}

    std::string_view GetName() const { return m_Name; }
protected:
    std::string m_Name;
};
}