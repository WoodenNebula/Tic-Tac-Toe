#pragma once

#include "Events/Event.h"
#include "Events/InputEvents/KeyCodes.h"

#ifdef DEBUG
#include "Core/Util/magic_enum.hpp"
#endif

#include <string>

namespace Engine::Events::InputEvents
{

class CKeyEvent : public CEventBase
{
public:
    EKey GetKey() const { return m_Key; }
    virtual EEventTypes GetEventType() const override { return m_EventType; }
    virtual EEventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    CKeyEvent() = delete;
    CKeyEvent(EKey Key, EEventTypes eventType) : CEventBase(eventType, EEventCategoryTypes::Key), m_Key(Key)
    {
    }
protected:
    EKey m_Key;
};


class CKeyPressedEvent : public CKeyEvent
{
public:
    CKeyPressedEvent(EKey Key, bool IsRepeat)
        :CKeyEvent(Key, GetStaticEventType()), m_IsRepeat(IsRepeat)
    {
    }

    static EEventTypes GetStaticEventType() { return EEventTypes::KeyPressed; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyPressedEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        msg.append(", IsRepeat: " + std::to_string(m_IsRepeat));
        return msg;
#else
        return "KeyPressedEvent: " + std::to_string(static_cast<std::underlying_type_t<EKey>>(m_Key)) + ", IsRepeat" + std::to_string(m_IsRepeat);
#endif
    }

    bool IsRepeat() const { return m_IsRepeat; }
private:
    bool m_IsRepeat;
};

class CKeyReleasedEvent : public CKeyEvent
{
public:
    CKeyReleasedEvent(EKey Key) :CKeyEvent(Key, GetStaticEventType()) {}

    static EEventTypes GetStaticEventType() { return EEventTypes::KeyReleased; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyReleasedEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        return msg;
#else
        return "KeyReleasedEvent: " + std::to_string(static_cast<std::underlying_type_t<EKey>>(m_Key));
#endif
    }
};

class CKeyHeldEvent : public CKeyEvent
{
public:
    CKeyHeldEvent(EKey Key) :CKeyEvent(Key, GetStaticEventType()) {}
    static EEventTypes GetStaticEventType() { return EEventTypes::KeyHeld; }

    virtual std::string ToString() const override
    {
#ifdef DEBUG
        std::string msg("KeyHeldEvent: ");
        msg.append((magic_enum::enum_name(m_Key)));
        msg.append(", IsRepeat: " + std::to_string(m_RepeatCount));
        return msg;
#else
        return "KeyHeldEvent: " + std::to_string(static_cast<std::underlying_type_t<EKey>>(m_Key)) + ", IsRepeat" + std::to_string(m_RepeatCount);
#endif 
    }

    int RepeatCount() const { return m_RepeatCount; }
protected:
    int m_RepeatCount{ 1 };

};
}; // namespace Engine::Events::InputEvents