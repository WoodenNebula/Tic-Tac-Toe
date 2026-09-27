#include "Networking/NetworkSubsystem.h"

#include "Error/Error.h"
#include "Logger/Logger.h"

#include "Networking/Sockets/SocketsAPI.h"
#include "Networking/Sockets/Sockets.h"

#include "Events/Event.h"
#include "Networking/Events/NetworkEvents.h"

#include <thread>
#include <memory>
#include <format>
#include <string>

DECLARE_LOG_CATEGORY(NetworkSubsystem);

namespace Engine
{
namespace Networking
{

CNetworkSubsystem& CNetworkSubsystem::Get()
{
    if (!CNetworkSubsystem::NetworkSubsystem)
        CNetworkSubsystem::NetworkSubsystem = new CNetworkSubsystem();

    return *CNetworkSubsystem::NetworkSubsystem;
}

SGenericError CNetworkSubsystem::Init()
{
    if (m_bIsInitialized)
        return { true, "" };

    bool bAPIInit = Sockets::CSocketAPI::Init();
    if (!bAPIInit)
    {
        LOG(LogNetworkSubsystem, Fatal, "Network Sockets API initialization failed!");
        return { false, "Network Sockets API initialization failed" };

    }

    LOG(LogNetworkSubsystem, Success, "Network Subsystem initialized!");

    m_bIsInitialized = bAPIInit;
    return { m_bIsInitialized, "" };
}

void CNetworkSubsystem::Shutdown()
{
    if (!m_bIsInitialized)
        return;

    m_ConnectionThread.request_stop();
    m_ReceiverThread.request_stop();
    m_SenderThread.request_stop();

    //m_ConnectionThread = {};
    //m_ReceiverThread = {};
    //m_SenderThread = {};

    if (m_ConnectionSocket)
        m_ConnectionSocket->Shutdown();

    LOG(LogNetworkSubsystem, Success, "Network Subsystem shutdown!");
}


void CNetworkSubsystem::OnEvent(Engine::Events::CEventBase& event)
{
    LOG(LogNetworkSubsystem, Info, "Transferring network event to application");
    m_AppCallback(event);
}

void CNetworkSubsystem::Update()
{
    std::lock_guard lock(m_NetworkEventMutex);

    while (!m_NetworkEventQueue.empty())
    {
        std::unique_ptr<Engine::Events::CEventBase> networkEvent;
        networkEvent = std::move(m_NetworkEventQueue.front());

        Engine::Events::CEventDispatcher dispatcher(*(networkEvent.get()));
        dispatcher.DispatchEvent<Events::CNetworkClientConnectedEvent>(BIND_EVENT_CB(CNetworkSubsystem::OnClientConnected));

        OnEvent(*networkEvent);

        m_NetworkEventQueue.pop();
    }
    //LOG(LogNetworkSubsystem, Trace, "Network Event Queried");
}

void CNetworkSubsystem::SetNetworkEventCallbackForApp(const NetworkEventCallbackFn& callback)
{
    m_AppCallback = callback;
    LOG(LogNetworkSubsystem, Trace, "Window Event Callback Set");
}

void CNetworkSubsystem::SendPayload(const Sockets::SSocketPayload& payload)
{
    m_SendingPayload = payload;
    bHasSendingPayload = true;
}

void CNetworkSubsystem::Async_EstablishConnection(std::stop_token stopToken)
{
    while (!stopToken.stop_requested())
    {
        {// mutex
            std::lock_guard lock(m_NetworkStatusMutex);

            if (m_ConnectionStatus == ENetworkConnectionStatus::Connected)
            {
                m_ConnectionThread.request_stop();
                return;
            }

            m_ConnectionStatus = ENetworkConnectionStatus::WaitingForConnection;
        }

        if (m_NetworkRole == ENetworkRole::Host)
        {
            LOG(LogNetworkSubsystem, Info, "Waiting to accept a client connection");
            auto client = m_ConnectionSocket->Accept();

            {// mutex access
                std::lock_guard lock(m_NetworkEventMutex);

                m_NetworkEventQueue.push(
                    std::make_unique< Events::CNetworkClientConnectedEvent>(std::move(client))
                );
            }
        }
        else if (m_NetworkRole == ENetworkRole::Client)
        {
            LOG(LogNetworkSubsystem, Info, "Trying to join a host connection");

            if (!m_ConnectionSocket)
                return;

            m_ConnectionSocket->Connect();
            {// mutex access
                LOG(LogNetworkSubsystem, Trace, "Join mutex acquired");
                std::lock_guard lock(m_NetworkEventMutex);


                m_NetworkEventQueue.push(
                    std::make_unique<Events::CNetworkClientConnectedEvent>(std::move(m_ConnectionSocket))
                );
            }
        }
        else
        {
            LOG(LogNetworkSubsystem, Warning, "Network role not established yet for establishing connection");
        }
    }
}

void CNetworkSubsystem::Async_ReceivePayload(
    std::stop_token stopToken,
    std::shared_ptr<Sockets::CSocket> connectionSocket,
    Sockets::SSocketPayload& outPayload
)
{
    using EResponse = Sockets::ERecieveResponse;
    while (!stopToken.stop_requested())
    {
        Sockets::SSocketPayload payload;

        EResponse Response = EResponse::Ok;
        while (true)
        {
            {// mutex
                std::lock_guard lock(m_NetworkStatusMutex);

                if (m_ConnectionStatus != ENetworkConnectionStatus::Connected || !connectionSocket)
                {
                    return;
                }
            }

            LOG(LogNetworkSubsystem, Info, "Async receiving payload");
            Response = connectionSocket->Receive(outPayload);

            if (Response != EResponse::Ok)
            {
                {// mutex access
                    std::lock_guard lock(m_NetworkEventMutex);
                    m_NetworkEventQueue.push(
                        std::make_unique<Events::CNetworkClientDisconnectedEvent>(std::move(connectionSocket))
                    );
                }

                //Print("{}Remote shutdown!{}", COLOR_YELLOW,  COLOR_RESET);
                break; // exit from thread by reaching the end of the thread's lambda
            }

            {// mutex access
                std::lock_guard lock(m_NetworkEventMutex);
                m_NetworkEventQueue.push(
                    std::make_unique<Events::CNetworkPacketReceivedEvent >(payload)
                );
            }

            //Print("-> {}", payload.BufferAsString());
        }
        //Print({}, "Receiver finished");

    }

}

void CNetworkSubsystem::Async_SendPayload(
    std::stop_token stopToken,
    std::shared_ptr<Sockets::CSocket> connectionSocket,
    const Sockets::SSocketPayload& payload
)
{
    while (!stopToken.stop_requested())
    {
        while (bHasSendingPayload)
        {
            {// mutex
                std::lock_guard lock(m_NetworkStatusMutex);

                if (m_ConnectionStatus != ENetworkConnectionStatus::Connected || !connectionSocket)
                {
                    return;
                }
            }

            LOG(LogNetworkSubsystem, Info, "Async sending payload");
            if (!connectionSocket->Send(payload))
            {
                // sending failed, most likely client disconnected
                {// mutex access
                    std::lock_guard lock(m_NetworkEventMutex);
                    m_NetworkEventQueue.push(
                        std::make_unique<Events::CNetworkClientDisconnectedEvent>(std::move(connectionSocket))
                    );
                }
                break;
            }

            {// mutex access
                std::lock_guard lock(m_NetworkEventMutex);
                m_NetworkEventQueue.push(
                    std::make_unique<Events::CNetworkPacketSentEvent>(payload)
                );
            }
        }
    }
}

void CNetworkSubsystem::Host()
{
    if (m_NetworkRole != ENetworkRole::None)
    {
        LOG(LogNetworkSubsystem, Error, "Already initialized as {}, reinitalization is not supported right now", ToString(m_NetworkRole));
        return;
    }

    m_NetworkRole = ENetworkRole::Host;

    Sockets::SAddress HostAddress;
    HostAddress.Port = GetDefaultPort();

    LOG(LogNetworkSubsystem, Info, "Started app as Host on address {}", HostAddress.ToString());

    m_ConnectionSocket->Init(HostAddress);
    m_ConnectionSocket->Bind();
    m_ConnectionSocket->Listen();

    m_ConnectionThread = std::jthread(
        [this](std::stop_token stopToken) {
            Async_EstablishConnection(stopToken);
        });
}


bool CNetworkSubsystem::OnClientConnected(Events::CNetworkClientConnectedEvent& event)
{
    m_ConnectionStatus = ENetworkConnectionStatus::Connected;

    auto Connection = event.GetConnectionSocket();
    m_ReceiverThread = std::jthread([this, Connection](std::stop_token stopToken) {
        Async_ReceivePayload(stopToken, Connection, m_ReceivedPayload);
        });

    m_SenderThread = std::jthread([this, Connection](std::stop_token stopToken) {
        Async_SendPayload(stopToken, Connection, m_SendingPayload);
        });

    // let the event pass through
    return false;
}

void CNetworkSubsystem::Join()
{
    Sockets::SAddress HostAddress = PromptRoomAddress();
    if (m_NetworkRole != ENetworkRole::None)
    {
        LOG(LogNetworkSubsystem, Error, "Already initialized as {}, reinitalization is not supported right now", ToString(m_NetworkRole));
        return;
    }

    m_NetworkRole = ENetworkRole::Client;

    LOG(LogNetworkSubsystem, Info, "Started app as Client to Join address {}", HostAddress.ToString());

    m_ConnectionSocket->Init(HostAddress);

    m_ConnectionThread = std::jthread(
        [this](std::stop_token stopToken) {
            Async_EstablishConnection(stopToken);
        });
}

Sockets::SAddress CNetworkSubsystem::PromptRoomAddress()
{
    std::string address;
    Sockets::SAddress HostAddress;
    while (true)
    {
        Print("Enter room address to join('d' for 127.0.0.1): example -> 192.168.1.1");
        std::cin >> address;
        if (address.at(0) == 'd')
            HostAddress = Sockets::SAddress::FromString(Sockets::SAddress::DefaultServerAddress());
        else
        {
            HostAddress = Sockets::SAddress::FromString(std::format("{}:{}", address, GetDefaultPort()));
        }
        if (HostAddress.IsValid())
            return HostAddress;
        else
        {
            Print("{}====\n{}{} is not a valid address", COLOR_YELLOW, COLOR_RED, HostAddress.ToString());
            Print("{}Please enter a valid address\n===={}", COLOR_YELLOW, COLOR_RESET);
        }
    }

    return {};
}

}   // namespace Networking
}   // namespace Engine