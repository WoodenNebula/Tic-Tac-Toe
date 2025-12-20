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

    std::string ToString() const override
    {
        return "WindowClosedEvent: Attempted to close window";
    }
};

class WindowGainFocusEvent : public WindowEvent
{
public:
    WindowGainFocusEvent()
    {
        m_EventType = EventTypes::WindowGainFocus;
    }

    std::string ToString() const override
    {
        return "WindowGainFocusEvent: Focused";
    }
};

class WindowLostFocus : public WindowEvent
{
public:
    WindowLostFocus()
    {
        m_EventType = EventTypes::WindowLostFocus;
    }

    std::string ToString() const override
    {
        return "WindowLostFocus: Lost Focus";
    }
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_Dimensions{ width, height }
    {
        m_EventType = EventTypes::WindowResize;
    }

    std::string ToString() const override
    {
        return "WindowResizedEvent: " + m_Dimensions.ToString();
    }
private:
    Point2D<uint32_t> m_Dimensions;
};

class WindowMovedEvent : public WindowEvent
{
public:
    WindowMovedEvent(int width, int height) : m_Position{ width, height }
    {
        m_EventType = EventTypes::WindowMoved;
    }

    std::string ToString() const override
    {
        return "WindowMovedEvent: " + m_Position.ToString();
    }

private:
    Point2D<int> m_Position;
};


}; // namespace Engine