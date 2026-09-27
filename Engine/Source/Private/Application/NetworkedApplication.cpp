#include "Application/NetworkedApplication.h"

#include "Networking/Sockets/Sockets.h"
#include "Networking/NetworkSubsystem.h"
#include "Networking/Sockets/SocketsAPI.h"

#include "Logger/Logger.h"

#include <string_view>
#include <iostream>
#include <format>
#include <thread>

namespace Engine
{
DECLARE_LOG_CATEGORY(NetworkedApplication);

CNetworkedApplication::CNetworkedApplication(const SApplicationProps& appProps, Networking::ENetworkRole networkRole)
    :CApplication(appProps), m_RequestedNetworkRole(networkRole)
{
}

CNetworkedApplication::~CNetworkedApplication()
{
    Networking::CNetworkSubsystem::Get().Shutdown();
    CApplication::Shutdown();
}

SGenericError CNetworkedApplication::Init()
{
    auto& NetSystem = Networking::CNetworkSubsystem::Get();
    NetSystem.Init();
    NetSystem.SetNetworkEventCallbackForApp(BIND_EVENT_CB(CNetworkedApplication::OnEvent));

    switch (m_RequestedNetworkRole)
    {
    case Engine::Networking::ENetworkRole::Client:
        NetSystem.Join();
        m_ApplicationProps.WindowProps.Title = "Tic-Tac-Toe (Client)";
        break;
    case Engine::Networking::ENetworkRole::Host:
        NetSystem.Host();
        m_ApplicationProps.WindowProps.Title = "Tic-Tac-Toe (Host)";
        break;
    case Engine::Networking::ENetworkRole::None:
    default:
        m_ApplicationProps.WindowProps.Title = "Tic-Tac-Toe";
        break;
    }

    return CApplication::Init();
}

void CNetworkedApplication::OnEvent(Events::CEventBase& event)
{
    // existing logic should be able to route network events
    CApplication::OnEvent(event);
}

void CNetworkedApplication::Run()
{
    while (m_IsRunning)
    {
        Update();
        Networking::CNetworkSubsystem::Get().Update();
        ProcessPendingOperations();
    }
    //networkedThread.join();
}

std::string CNetworkedApplication::PromptUsername()
{
    std::string username;
    Print("Enter your username: ");
    std::cin >> username;
    return username;
}
};
