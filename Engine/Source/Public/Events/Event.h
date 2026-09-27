#pragma once
#include "Logger/Logger.h"


#include <string>
#include <functional>
#include <concepts>


namespace Engine::Events
{

DECLARE_LOG_CATEGORY_LEVEL(EventManager, Warning)

enum class EEventTypes
{
    None = 0,
    WindowClose, WindowResize, WindowGainFocus, WindowLostFocus, WindowMoved,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseEntered,
    KeyPressed, KeyReleased, KeyHeld,
    ApplicationTick, ApplicationUpdate, ApplicationRender,
    NetworkHosted, NetworkClientConnected, NetworkClientDisconnected, NetworkPacketReceived, NetworkPacketSent
};

enum class EEventCategoryTypes
{
    None = 0,
    Window,
    Mouse,
    Key,
    Application,
    Network
};

class CEventBase
{
public:
    virtual ~CEventBase() = default;

    virtual EEventTypes GetEventType() const = 0;
    virtual EEventCategoryTypes GetEventCategory() const = 0;
    virtual std::string ToString() const = 0;

public:
    bool Handled{ false };

protected:
    CEventBase() {}
    CEventBase(EEventTypes eventType, EEventCategoryTypes CategoryType) :
        m_EventType(eventType), m_EventCategory(CategoryType)
    {
    }
protected:
    EEventTypes m_EventType{ EEventTypes::None };
    EEventCategoryTypes m_EventCategory{ EEventCategoryTypes::None };
};

template <typename T>
concept EventType = std::is_base_of<CEventBase, T>::value&& requires {
    {  T::GetStaticEventType() } -> std::same_as<EEventTypes>;
};

template<EventType T>
using EventCallbackFn = std::function<bool(T&)>;

#define BIND_EVENT_CB(X) (std::bind(&X, this, std::placeholders::_1))

class CEventDispatcher
{

public:
    CEventDispatcher(CEventBase& e) : m_Event(e) {}

    template<EventType T>
    bool DispatchEvent(EventCallbackFn<T> cb)
    {
        if (T::GetStaticEventType() == m_Event.GetEventType())
        {
            switch (m_Event.GetEventCategory())
            {
            case EEventCategoryTypes::Window:
                LOG(LogEventManager, Trace, "Dispatched Window Event {}", m_Event.ToString());
                break;
            case EEventCategoryTypes::Mouse:
                LOG(LogEventManager, Trace, "Dispatched Mouse Event {}", m_Event.ToString());
                break;
            case EEventCategoryTypes::Key:
                LOG(LogEventManager, Trace, "Dispatched Key Event {}", m_Event.ToString());
                break;
            case EEventCategoryTypes::Application:
                LOG(LogEventManager, Trace, "Dispatched Application Event {}", m_Event.ToString());
                break;
            case EEventCategoryTypes::Network:
                LOG(LogEventManager, Trace, "Dispatched Netowkr Event {}", m_Event.ToString());
                break;
            default:
                LOG(LogEventManager, Warning, "Dispatched Unknown Event Category {}", m_Event.ToString());
                break;
            }


            m_Event.Handled |= std::invoke(cb, static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }
protected:
    CEventBase& m_Event;
};

}; // namespace Engine::Events

template<>
struct std::formatter<Engine::Events::CEventBase> : std::formatter<std::string>
{
    auto format(const Engine::Events::CEventBase& event, auto& ctx) const
    {
        return std::formatter<std::string>::format(std::format("{{ {} }}", event.ToString()), ctx);
    }
};


