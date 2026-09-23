#pragma once

#include "Events/Event.h"
#include <string>

namespace Engine
{
class CLayer
{
public:
    CLayer(std::string_view Name = "Layer") : m_Name(Name) {}
    virtual ~CLayer() = default;

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnEvent(Events::CEventBase& event) {}

    std::string_view GetName() const { return m_Name; }
protected:
    std::string m_Name;
};
}