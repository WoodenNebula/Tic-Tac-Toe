#pragma once
#include "Logger/Logger.h"


#include <string>
#include <functional>
#include <concepts>


namespace Engine::Events
{

DECLARE_LOG_CATEGORY(EventManager)

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
    virtual ~EventBase() = default;

    virtual EventTypes GetEventType() const = 0;
    virtual EventCategoryTypes GetEventCategory() const = 0;
    virtual std::string ToString() const = 0;

public:
    bool Handled{ false };

protected:
    EventBase() {}
    EventBase(EventTypes eventType, EventCategoryTypes CategoryType) :
        m_EventType(eventType), m_EventCategory(CategoryType)
    {
    }
protected:
    EventTypes m_EventType{ EventTypes::None };
    EventCategoryTypes m_EventCategory{ EventCategoryTypes::None };
};

template <typename T>
concept EventType = std::is_base_of<EventBase, T>::value&& requires {
    {  T::GetStaticEventType() } -> std::same_as<EventTypes>;
};

template<EventType T>
using EventCallbackFn = std::function<bool(T&)>;

#define BIND_EVENT_CB(X) (std::bind(&X, this, std::placeholders::_1))

class EventDispatcher
{

public:
    EventDispatcher(EventBase& e) : m_Event(e) {}

    template<EventType T>
    bool DispatchEvent(EventCallbackFn<T> cb)
    {
        if (T::GetStaticEventType() == m_Event.GetEventType())
        {
            switch (m_Event.GetEventCategory())
            {
            case EventCategoryTypes::Window:
                LOG(EventManager, INFO, "Dispatched Window Event {}", m_Event.ToString());
                break;
            case EventCategoryTypes::Mouse:
                LOG(EventManager, INFO, "Dispatched Mouse Event {}", m_Event.ToString());
                break;
            case EventCategoryTypes::Key:
                LOG(EventManager, INFO, "Dispatched Key Event {}", m_Event.ToString());
                break;
            case EventCategoryTypes::Application:
                LOG(EventManager, INFO, "Dispatched Application Event {}", m_Event.ToString());
                break;
            default:
                LOG(EventManager, WARN, "Dispatched Unknown Event Category {}", m_Event.ToString());
                break;
            }


            m_Event.Handled |= std::invoke(cb, static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
protected:
    EventBase& m_Event;
};

}; // namespace Engine::Events

template<>
struct std::formatter<Engine::Events::EventBase> : std::formatter<std::string>
{
    auto format(const Engine::Events::EventBase& event, auto& ctx) const
    {
        return std::formatter<std::string>::format(std::format("{{ {} }}", event.ToString()), ctx);
    }
};


