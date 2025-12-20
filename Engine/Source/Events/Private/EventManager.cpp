#include "Events/EventManager.h"
#include "Events/CoreEvents.h"
#include "Logger/Logger.h"

#include "Core/Util/magic_enum.hpp"

DECLARE_LOG_CATEGORY(EventManager)

namespace Engine::Events
{

void EventManager::Subscribe(EventTypes eventType, const EventCallbackFn cb)
{
    // Subscription logic would go here
    LOG(EventManager, INFO, "Subscribed to event type {}", magic_enum::enum_name<EventTypes>(eventType));
}


void EventManager::DispatchEvent(const EventBase& Event)
{
    // For demonstration purposes, we will just print the event type
    switch (Event.GetEventCategory())
    {
    case EventCategoryTypes::Window:
        LOG(EventManager, TRACE, "Dispatched Window Event {}", Event.ToString());
        break;
    case EventCategoryTypes::Mouse:
        // LOG(EventManager, TRACE, "Dispatched Mouse Event {}", Event.ToString());
        break;
    case EventCategoryTypes::Key:
        LOG(EventManager, TRACE, "Dispatched Key Event {}", Event.ToString());
        break;
    case EventCategoryTypes::Application:
        LOG(EventManager, TRACE, "Dispatched Application Event {}", Event.ToString());
        break;
    default:
        LOG(EventManager, WARN, "Dispatched Unknown Event Category {}", Event.ToString());
        break;
    }
}
};