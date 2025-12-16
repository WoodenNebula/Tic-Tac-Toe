#include "Events/EventManager.h"
#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"
#include "Logger/Logger.h"

namespace Engine::Events
{

void EventManager::Subscribe(EventTypes eventType, const EventCallbackFn cb)
{
    // Subscription logic would go here
    Logger::LogInfo("EventManager", "Subscribed to event type");
}


void EventManager::DispatchEvent(const EventBase& Event)
{
    // For demonstration purposes, we will just print the event type
    switch (Event.GetEventCategory())
    {
    case EventCategoryTypes::Window:
        Logger::LogInfo("EventManager", "Dispatched Window Event " + Event.ToString());
        break;
    case EventCategoryTypes::Mouse:
        Logger::LogInfo("EventManager", "Dispatched Mouse Event " + Event.ToString());
        break;
    case EventCategoryTypes::Key:
        Logger::LogInfo("EventManager", "Dispatched Key Event " + Event.ToString());
        break;
    case EventCategoryTypes::Application:
        Logger::LogInfo("EventManager", "Dispatched Application Event " + Event.ToString());
        break;
    default:
        Logger::LogWarning("EventManager", "Dispatched Unknown Event Category " + Event.ToString());
        break;
    }
}
};