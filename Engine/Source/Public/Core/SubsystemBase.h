#pragma once

#include "Error/Error.h"
#include "Events/CoreEvents.h"

namespace Engine
{
class ISubsystem
{
protected:
    ISubsystem() {};
public:
    virtual ~ISubsystem() {};

    virtual SGenericError Init() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;

    virtual void OnEvent(Events::CEventBase& event) = 0;
};
}   // namespace Engine
