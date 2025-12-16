#pragma once

#include "Events/Event.h"
#include "Events/MouseCodes.h"

#include "Core/Util/Utils.h"

#ifdef DEBUG
#include "Core/Util/magic_enum.hpp"
#endif

#include <string>

namespace Engine::Events::InputEvents
{

class MouseEvent : public EventBase
{
protected:
    MouseEvent() : EventBase(EventTypes::None, EventCategoryTypes::Mouse) {}
};

class MouseButtonPressedEvent : public MouseEvent
{
public:
    MouseButtonPressedEvent(Mouse Button) : m_Button(Button)
    {
        m_EventType = EventTypes::MouseButtonPressed;
    }

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
    MouseButtonReleasedEvent(Mouse Button) : m_Button(Button)
    {
        m_EventType = EventTypes::MouseButtonReleased;
    }

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

    MouseMovedEvent(double xPos, double yPos) : m_Pos{ xPos, yPos }
    {
        m_EventType = EventTypes::MouseMoved;
    }

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

    MouseScrolledEvent(double xOffset, double yOffset) : m_Offset{ xOffset, yOffset }
    {
        m_EventType = EventTypes::MouseScrolled;
    }

    virtual std::string ToString() const override
    {
        return "MouseScrolledEvent: " + m_Offset.ToString();
    }
    Position GetOffset() const { return m_Offset; }
private:
    Position m_Offset;
};

}; // namespace Engine::Events::InputEvents