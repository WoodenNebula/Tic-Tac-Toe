#pragma once

#include "Events/Event.h"
#include "Events/InputEvents/MouseCodes.h"

#include "Core/Util/Utils.h"

#ifdef DEBUG
#include "Core/Util/magic_enum.hpp"
#endif

#include <string>

namespace Engine::Events::InputEvents
{

class MouseEvent : public EventBase
{
public:
    virtual EventTypes GetEventType() const override { return m_EventType; }
    virtual EventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    MouseEvent(EventTypes eventType) : EventBase(eventType, EventCategoryTypes::Mouse) {}
    virtual ~MouseEvent() = default;
};

class MouseButtonPressedEvent : public MouseEvent
{
public:
    MouseButtonPressedEvent(Mouse Button) : MouseEvent(GetStaticEventType()), m_Button(Button)
    {
        m_EventType = EventTypes::MouseButtonPressed;
    }

    static EventTypes GetStaticEventType() { return EventTypes::MouseButtonPressed; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("MouseButtonPressedEvent: ");
        msg.append((magic_enum::enum_name(m_Button)));
        return msg;
#else
        return "MouseButtonPressedEvent: " + std::to_string(static_cast<std::underlying_type_t<Mouse>>(m_Button));
#endif
    }

    Mouse GetButton() const { return m_Button; }
private:
    Mouse m_Button;
};

class MouseButtonReleasedEvent : public MouseEvent
{
public:
    MouseButtonReleasedEvent(Mouse Button) : MouseEvent(GetStaticEventType()), m_Button(Button)
    {
    }

    static EventTypes GetStaticEventType() { return EventTypes::MouseButtonReleased; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("MouseButtonReleasedEvent: ");
        msg.append((magic_enum::enum_name(m_Button)));
        return msg;
#else
        return "MouseButtonReleasedEvent: " + std::to_string(static_cast<std::underlying_type_t<Mouse>>(m_Button));
#endif
    }

    Mouse GetButton() const { return m_Button; }
private:
    Mouse m_Button;
};

class MouseMovedEvent : public MouseEvent
{
public:
    using Position = Point2D<double>;

    MouseMovedEvent(double xPos, double yPos) : MouseEvent(GetStaticEventType()), m_Pos{ xPos, yPos } {}

    static EventTypes GetStaticEventType() { return EventTypes::MouseMoved; }

    virtual std::string ToString() const override
    {
        return "MouseMovedEvent: " + m_Pos.ToString();
    }

    Position GetPos() const { return m_Pos; }
private:
    Position m_Pos;
};

class MouseScrolledEvent : public MouseEvent
{
public:
    using Position = Point2D<double>;

    MouseScrolledEvent(double xOffset, double yOffset) : MouseEvent(GetStaticEventType()), m_Offset{ xOffset, yOffset } {}

    static EventTypes GetStaticEventType() { return EventTypes::MouseScrolled; }

    virtual std::string ToString() const override
    {
        return "MouseScrolledEvent: " + m_Offset.ToString();
    }
    Position GetOffset() const { return m_Offset; }
private:
    Position m_Offset;
};

}; // namespace Engine::Events::InputEvents