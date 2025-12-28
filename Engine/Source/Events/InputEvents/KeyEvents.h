#pragma once

#include "Events/Event.h"
#include "Events/InputEvents/KeyCodes.h"

#ifdef DEBUG
#include "Core/Util/magic_enum.hpp"
#endif

#include <string>

namespace Engine::Events::InputEvents
{

class KeyEvent : public EventBase
{
public:
    Key GetKey() const { return m_Key; }
    virtual EventTypes GetEventType() const override { return m_EventType; }
    virtual EventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    KeyEvent() = delete;
    KeyEvent(Key Key, EventTypes eventType) : EventBase(eventType, EventCategoryTypes::Key), m_Key(Key)
    {
    }
protected:
    Key m_Key;
};


class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(Key Key, bool IsRepeat)
        :KeyEvent(Key, GetStaticEventType()), m_IsRepeat(IsRepeat)
    {
    }

    static EventTypes GetStaticEventType() { return EventTypes::KeyPressed; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyPressedEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        msg.append(", IsRepeat: " + std::to_string(m_IsRepeat));
        return msg;
#else
        return "KeyPressedEvent: " + std::to_string(static_cast<std::underlying_type_t<Key>>(m_Key)) + ", IsRepeat" + std::to_string(m_IsRepeat);
#endif
    }

    bool IsRepeat() const { return m_IsRepeat; }
private:
    bool m_IsRepeat;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(Key Key) :KeyEvent(Key, GetStaticEventType()) {}

    static EventTypes GetStaticEventType() { return EventTypes::KeyReleased; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyReleasedEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        return msg;
#else
        return "KeyReleasedEvent: " + std::to_string(static_cast<std::underlying_type_t<Key>>(m_Key));
#endif
    }
};

class KeyHeldEvent : public KeyEvent
{
public:
    KeyHeldEvent(Key Key) :KeyEvent(Key, GetStaticEventType()) {}
    static EventTypes GetStaticEventType() { return EventTypes::KeyHeld; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyHeldEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        msg.append(", IsRepeat: " + std::to_string(m_RepeatCount));
        return msg;
#else
        return "KeyHeldEvent: " + std::to_string(static_cast<std::underlying_type_t<Key>>(m_Key)) + ", IsRepeat" + std::to_string(m_RepeatCount);
#endif 
    }

    int RepeatCount() const { return m_RepeatCount; }
protected:
    int m_RepeatCount{ 1 };

};
}; // namespace Engine::Events::InputEvents