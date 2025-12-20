#pragma once
#include <string>

namespace Engine::Events
{
enum class EventTypes
{
    None = 0,
    WindowClose, WindowResize, WindowGainFocus, WindowLostFocus, WindowMoved,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseEntered,
    KeyPressed, KeyReleased, KeyHeld,
    ApplicationTick, ApplicationUpdate, ApplicationRender
};

enum class EventCategoryTypes
{
    None = 0,
    Window,
    Mouse,
    Key,
    Application
};

class EventBase
{
public:
    EventBase() {}
    EventBase(EventTypes EventType, EventCategoryTypes CategoryType) :
        m_EventType(EventType), m_EventCategory(CategoryType)
    {
    }
    virtual ~EventBase() = default;

    EventTypes GetEventType() const { return m_EventType; }
    EventCategoryTypes GetEventCategory() const { return m_EventCategory; }

    virtual std::string ToString() const = 0;
protected:
    EventTypes m_EventType{ EventTypes::None };
    EventCategoryTypes m_EventCategory{ EventCategoryTypes::None };
};

}; // namespace Engine::Events