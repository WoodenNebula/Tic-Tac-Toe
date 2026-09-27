#pragma once

#include "Application/Application.h"

#include "Networking/NetworkSubsystem.h"
#include "Networking/Sockets/Sockets.h"
#include "Error/Error.h"

#include <thread>
#include <string>

namespace Engine
{

class CNetworkedApplication : public CApplication
{
public:
    CNetworkedApplication(const SApplicationProps& appProps, Networking::ENetworkRole networkRole);
    ~CNetworkedApplication();

    virtual SGenericError Init() override;
    virtual void Run() override;
    virtual void OnEvent(Events::CEventBase& event) override;

public:
    static std::string PromptUsername();
protected:
    Networking::ENetworkRole m_RequestedNetworkRole{ Networking::ENetworkRole::None };
};
}   // namespace Engine
