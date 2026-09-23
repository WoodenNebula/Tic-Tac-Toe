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

class CMouseEvent : public CEventBase
{
public:
    virtual EEventTypes GetEventType() const override { return m_EventType; }
    virtual EEventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    CMouseEvent(EEventTypes eventType) : CEventBase(eventType, EEventCategoryTypes::Mouse) {}
    virtual ~CMouseEvent() = default;
};

class CMouseButtonPressedEvent : public CMouseEvent
{
public:
    CMouseButtonPressedEvent(EMouse Button) : CMouseEvent(GetStaticEventType()), m_Button(Button)
    {
        m_EventType = EEventTypes::MouseButtonPressed;
    }

    static EEventTypes GetStaticEventType() { return EEventTypes::MouseButtonPressed; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("MouseButtonPressedEvent: ");
        msg.append((magic_enum::enum_name(m_Button)));
        return msg;
#else
        return "MouseButtonPressedEvent: " + std::to_string(static_cast<std::underlying_type_t<EMouse>>(m_Button));
#endif
    }

    EMouse GetButton() const { return m_Button; }
private:
    EMouse m_Button;
};

class CMouseButtonReleasedEvent : public CMouseEvent
{
public:
    CMouseButtonReleasedEvent(EMouse Button) : CMouseEvent(GetStaticEventType()), m_Button(Button)
    {
    }

    static EEventTypes GetStaticEventType() { return EEventTypes::MouseButtonReleased; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("MouseButtonReleasedEvent: ");
        msg.append((magic_enum::enum_name(m_Button)));
        return msg;
#else
        return "MouseButtonReleasedEvent: " + std::to_string(static_cast<std::underlying_type_t<EMouse>>(m_Button));
#endif
    }

    EMouse GetButton() const { return m_Button; }
private:
    EMouse m_Button;
};

class CMouseMovedEvent : public CMouseEvent
{
public:
    using Position = Point2D<double>;

    CMouseMovedEvent(double xPos, double yPos) : CMouseEvent(GetStaticEventType()), m_Pos{ xPos, yPos } {}

    static EEventTypes GetStaticEventType() { return EEventTypes::MouseMoved; }

    virtual std::string ToString() const override
    {
        return "MouseMovedEvent: " + m_Pos.ToString();
    }

    Position GetPos() const { return m_Pos; }
private:
    Position m_Pos;
};

class CMouseScrolledEvent : public CMouseEvent
{
public:
    using Position = Point2D<double>;

    CMouseScrolledEvent(double xOffset, double yOffset) : CMouseEvent(GetStaticEventType()), m_Offset{ xOffset, yOffset } {}

    static EEventTypes GetStaticEventType() { return EEventTypes::MouseScrolled; }

    virtual std::string ToString() const override
    {
        return "MouseScrolledEvent: " + m_Offset.ToString();
    }
    Position GetOffset() const { return m_Offset; }
private:
    Position m_Offset;
};

}; // namespace Engine::Events::InputEvents