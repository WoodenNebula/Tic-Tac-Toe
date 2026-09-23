#pragma once

#include "Events/Event.h"

#include "Core/Util/Utils.h"

namespace Engine::Events
{

class CWindowEvent : public CEventBase
{
public:
    virtual EEventTypes GetEventType() const override { return m_EventType; }
    virtual EEventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    CWindowEvent(EEventTypes eventType) : CEventBase(eventType, EEventCategoryTypes::Window) {}
    virtual ~CWindowEvent() = default;
};

class CWindowCloseEvent : public CWindowEvent
{
public:
    CWindowCloseEvent() : CWindowEvent(GetStaticEventType()) {}

    static EEventTypes GetStaticEventType() { return EEventTypes::WindowClose; }

    std::string ToString() const override
    {
        return "WindowClosedEvent: Attempted to close window";
    }
};

class CWindowGainFocusEvent : public CWindowEvent
{
public:
    CWindowGainFocusEvent() : CWindowEvent(GetStaticEventType()) {}

    static EEventTypes GetStaticEventType() { return EEventTypes::WindowGainFocus; }

    std::string ToString() const override
    {
        return "WindowGainFocusEvent: Focused";
    }
};

class CWindowLostFocus : public CWindowEvent
{
public:
    CWindowLostFocus() : CWindowEvent(GetStaticEventType()) {}

    static EEventTypes GetStaticEventType() { return EEventTypes::WindowLostFocus; }

    std::string ToString() const override
    {
        return "WindowLostFocus: Lost Focus";
    }
};

class WindowResizeEvent : public CWindowEvent
{
public:
    WindowResizeEvent(uint32_t width, uint32_t height) : CWindowEvent(GetStaticEventType()), m_Dimensions{ width, height } {}

    static EEventTypes GetStaticEventType() { return EEventTypes::WindowResize; }

    Point2D<uint32_t> GetDimensions() const { return m_Dimensions; }

    std::string ToString() const override
    {
        return "WindowResizedEvent: " + m_Dimensions.ToString();
    }
private:
    Point2D<uint32_t> m_Dimensions;
};

class WindowMovedEvent : public CWindowEvent
{
public:
    WindowMovedEvent(int width, int height) : CWindowEvent(GetStaticEventType()), m_Position{ static_cast<int32_t>(width), static_cast<int32_t>(height) } {}

    static EEventTypes GetStaticEventType() { return EEventTypes::WindowMoved; }

    Point2D<int32_t> GetPosition() const { return m_Position; }

    std::string ToString() const override
    {
        return "WindowMovedEvent: " + m_Position.ToString();
    }

private:
    Point2D<int32_t> m_Position;
};


}; // namespace Engine