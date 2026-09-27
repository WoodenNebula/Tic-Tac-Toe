#pragma once

#include "Events/Event.h"
#include "Networking/NetworkSubsystem.h"
#include "Networking/Sockets/Sockets.h"

namespace Engine::Networking::Events
{

class CNetworkEventBase : public Engine::Events::CEventBase
{
public:
    virtual Engine::Events::EEventTypes GetEventType() const override { return m_EventType; }
    virtual Engine::Events::EEventCategoryTypes GetEventCategory() const override { return m_EventCategory; }
protected:
    CNetworkEventBase(Engine::Events::EEventTypes eventType) : CEventBase(eventType, Engine::Events::EEventCategoryTypes::Network) {}
};

class CNetworkHostedEvent : public CNetworkEventBase
{
public:
    CNetworkHostedEvent() : CNetworkEventBase(GetStaticEventType()) {}
    static Engine::Events::EEventTypes GetStaticEventType() { return Engine::Events::EEventTypes::NetworkHosted; }

    std::string ToString() const override
    {
        return "NetworkHostedEvent: Network Connection as host started";
    }
};
class CNetworkClientConnectedEvent : public CNetworkEventBase
{
public:
    CNetworkClientConnectedEvent(std::shared_ptr<Sockets::CSocket> Client) : CNetworkEventBase(GetStaticEventType()), m_ClientSocket(Client) {}
    static Engine::Events::EEventTypes GetStaticEventType() { return Engine::Events::EEventTypes::NetworkClientConnected; }

    std::string ToString() const override
    {
        return "NetworkJoinedEvent: Network client connected";
    }

    std::shared_ptr<Sockets::CSocket> GetConnectionSocket() const { return std::move(m_ClientSocket); }
private:
    std::shared_ptr<Sockets::CSocket> m_ClientSocket;
};
class CNetworkClientDisconnectedEvent : public CNetworkEventBase
{
public:
    CNetworkClientDisconnectedEvent(std::shared_ptr<Sockets::CSocket> Client) : CNetworkEventBase(GetStaticEventType()), m_ClientSocket(Client) {}
    static Engine::Events::EEventTypes GetStaticEventType() { return Engine::Events::EEventTypes::NetworkClientDisconnected; }

    std::string ToString() const override
    {
        return "NetworkJoinedEvent: Network client connected";
    }

    std::shared_ptr<Sockets::CSocket> GetConnectionSocket() const { return std::move(m_ClientSocket); }
private:
    std::shared_ptr<Sockets::CSocket> m_ClientSocket;
};
class CNetworkPacketReceivedEvent : public CNetworkEventBase
{
public:
    CNetworkPacketReceivedEvent(const Sockets::SSocketPayload& Payload) : CNetworkEventBase(GetStaticEventType()), m_Payload(Payload) {}
    static Engine::Events::EEventTypes GetStaticEventType() { return Engine::Events::EEventTypes::NetworkPacketReceived; }

    std::string ToString() const override
    {
        return "NetworkPacketReceivedEvent: Network Packet received";
    }

    inline Sockets::SSocketPayload GetPayload() const { return m_Payload; }
private:
    Sockets::SSocketPayload m_Payload;
};
class CNetworkPacketSentEvent : public CNetworkEventBase
{
public:
    CNetworkPacketSentEvent(const Sockets::SSocketPayload& Payload) : CNetworkEventBase(GetStaticEventType()), m_Payload(Payload) {}
    static Engine::Events::EEventTypes GetStaticEventType() { return Engine::Events::EEventTypes::NetworkPacketSent; }

    std::string ToString() const override
    {
        return "NetworkPacketSentEvent: Network Packet sent";
    }

    inline Sockets::SSocketPayload GetPayload() const { return m_Payload; }
private:
    Sockets::SSocketPayload m_Payload;
};
}
