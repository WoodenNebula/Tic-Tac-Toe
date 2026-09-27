#pragma once

#include "Error/Error.h"
#include "Networking/Sockets/Sockets.h"

#include "Networking/Events/NetworkEvents.h"

#include "Events/CoreEvents.h"
#include "Core/SubsystemBase.h"

#include <string>
#include <thread>
#include <memory>
#include <mutex>

#include <queue>

namespace Engine
{
namespace Networking
{
/// <summary>
/// Actions that the networking subsystem can perform. All actions are driven by these command types
/// </summary>
enum class ENetworkCommandType
{
    Listen,
    Connect,
    Send,
    Disconnect,
    Shutdown
};

enum class ENetworkConnectionStatus
{
    None,
    WaitingForConnection,
    Connected
};

enum class ENetworkEventType
{
    Listening,
    Connecting,
    Connected,
    ConnectionFailed,
    Disconnected,
    PacketReceived
};

enum class ENetworkRole
{
    None,
    Client,
    Host
};


using NetworkEventCallbackFn = std::function<void(Engine::Events::CEventBase&)>;

class CNetworkSubsystem : public ISubsystem
{
public:
    CNetworkSubsystem() : m_ConnectionSocket(std::make_shared<Sockets::CSocket>()) {};
    virtual ~CNetworkSubsystem() {};

    //////////////////////////////////
    ///* Subsystem base interface *///
    //////////////////////////////////
    virtual SGenericError Init() override;
    virtual void Update() override;
    virtual void Shutdown() override;

    virtual void OnEvent(Engine::Events::CEventBase& event) override;

    void SetNetworkEventCallbackForApp(const NetworkEventCallbackFn& callback);

    void SendPayload(const Sockets::SSocketPayload& payload);

    void Host();
    void Join();

    inline ENetworkRole GetCurrentNetworkRole() const { return m_NetworkRole; }
    inline ENetworkConnectionStatus GetConnectionStatus() const { return m_ConnectionStatus; }

public:
    static CNetworkSubsystem& Get();
    //static ENetworkRole PromptHostOrJoin();
    //static std::string PromptUsername();

protected:
    Sockets::SAddress PromptRoomAddress();
    static inline constexpr Sockets::SAddress::Port_t GetDefaultPort() { return 27020; }

    bool OnClientConnected(Events::CNetworkClientConnectedEvent& e);

private:
    void Async_EstablishConnection(std::stop_token stopToken);
    void Async_ReceivePayload(std::stop_token stopToken, std::shared_ptr<Sockets::CSocket> connectionSocket, Sockets::SSocketPayload& outPayload);
    void Async_SendPayload(std::stop_token stopToken, std::shared_ptr<Sockets::CSocket> connectionSocket, const Sockets::SSocketPayload& Payload);

private:
    static inline CNetworkSubsystem* NetworkSubsystem;
private:
    NetworkEventCallbackFn m_AppCallback;
    bool m_bIsInitialized{ false };

    ENetworkRole m_NetworkRole{ ENetworkRole::None };
    std::mutex m_NetworkStatusMutex;
    ENetworkConnectionStatus m_ConnectionStatus{ ENetworkConnectionStatus::None };

    std::mutex m_NetworkEventMutex;
    std::queue<std::unique_ptr<Engine::Events::CEventBase>> m_NetworkEventQueue;

    std::jthread m_ConnectionThread;
    std::jthread m_ReceiverThread;
    std::jthread m_SenderThread;

    std::shared_ptr<Sockets::CSocket> m_ConnectionSocket;

    Sockets::SSocketPayload m_ReceivedPayload;
    Sockets::SSocketPayload m_SendingPayload;

    std::atomic_bool bHasSendingPayload{ false };
};

}   // namespace Networking


static std::string ToString(const Networking::ENetworkRole RoomState)
{
    switch (RoomState)
    {
    case Networking::ENetworkRole::Client: return "Client";
    case Networking::ENetworkRole::Host: return "Host";
    case Networking::ENetworkRole::None: return "None";
    default: return "";
    }
}

}   // namespace Engine
