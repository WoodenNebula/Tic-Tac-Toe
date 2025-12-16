#pragma once

#include "Events/Event.h"

#include "Core/Util/Utils.h"

namespace Engine::Events
{

class WindowEvent : public EventBase
{
protected:
    WindowEvent() : EventBase(EventTypes::None, EventCategoryTypes::Window) {}
    ~WindowEvent() = default;
};

class WindowCloseEvent : public WindowEvent
{
public:
    WindowCloseEvent()
    {
        m_EventType = EventTypes::WindowClose;
    }
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_Dimensions{ width, height }
    {
        m_EventType = EventTypes::WindowResize;
    }

private:
    Point2D<uint32_t> m_Dimensions;
};


}; // namespace Engine