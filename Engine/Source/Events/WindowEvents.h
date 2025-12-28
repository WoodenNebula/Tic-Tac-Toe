#pragma once

#include "Events/Event.h"

#include "Core/Util/Utils.h"

namespace Engine::Events
{

class WindowEvent : public EventBase
{
public:
    virtual EventTypes GetEventType() const override { return m_EventType; }
    virtual EventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    WindowEvent(EventTypes eventType) : EventBase(eventType, EventCategoryTypes::Window) {}
    virtual ~WindowEvent() = default;
};

class WindowCloseEvent : public WindowEvent
{
public:
    WindowCloseEvent() : WindowEvent(GetStaticEventType()) {}

    static EventTypes GetStaticEventType() { return EventTypes::WindowClose; }

    std::string ToString() const override
    {
        return "WindowClosedEvent: Attempted to close window";
    }
};

class WindowGainFocusEvent : public WindowEvent
{
public:
    WindowGainFocusEvent() : WindowEvent(GetStaticEventType()) {}

    static EventTypes GetStaticEventType() { return EventTypes::WindowGainFocus; }

    std::string ToString() const override
    {
        return "WindowGainFocusEvent: Focused";
    }
};

class WindowLostFocus : public WindowEvent
{
public:
    WindowLostFocus() : WindowEvent(GetStaticEventType()) {}

    static EventTypes GetStaticEventType() { return EventTypes::WindowLostFocus; }

    std::string ToString() const override
    {
        return "WindowLostFocus: Lost Focus";
    }
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : WindowEvent(GetStaticEventType()), m_Dimensions{ width, height } {}

    static EventTypes GetStaticEventType() { return EventTypes::WindowResize; }

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
    WindowMovedEvent(int width, int height) : WindowEvent(GetStaticEventType()), m_Position{ width, height } {}

    static EventTypes GetStaticEventType() { return EventTypes::WindowMoved; }

    std::string ToString() const override
    {
        return "WindowMovedEvent: " + m_Position.ToString();
    }

private:
    Point2D<int> m_Position;
};


}; // namespace Engine