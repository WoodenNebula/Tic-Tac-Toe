#pragma once
#include "Core/Util/Singleton.h"
#include "Events/Event.h"

#include <functional>

namespace Engine::Events
{

using EventCallbackFn = std::function<void(const EventBase&)>;

class EventManager : public Singleton<EventManager>
{
public:
    void DispatchEvent(const EventBase& Event);

    void Subscribe(EventTypes eventType, const EventCallbackFn cb);
};
};