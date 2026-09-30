#include "TicTacToeLayers.h"

namespace Game
{
void CWaitingForNetwork_OverlayLayer::OnAttach()
{
    LOG(LogTicTacToe, Trace, "Layer Attached in {}", m_Name);
    m_WaitingTexture = std::make_shared<Engine::CTexture>("./res/textures/Waiting.png");
}


void CWaitingForNetwork_OverlayLayer::OnUpdate(float deltaTime)
{
    Engine::CRenderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Engine::CRenderer::Clear();

    Point2D<uint32_t> viewPortSize = CTicTacToe::Get().GetWindowDimensions();

    glm::vec3 centerPos{ 0.0f, 0.0f, 0.0f };
    glm::vec2 normalizedSize{ (float)256 / viewPortSize.x, (float)256 / viewPortSize.y };

    Engine::CRenderer::StartDraw();
    Engine::CRenderer::DrawSprite(centerPos, normalizedSize, "./res/textures/Waiting.png");
    Engine::CRenderer::Flush();

}
void CWaitingForNetwork_OverlayLayer::OnEvent(Engine::Events::CEventBase& event)
{
    LOG(LogTicTacToe, Trace, "Event {} Received in {}", event, m_Name);

    Engine::Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseButtonReleasedEvent>([this](Engine::Events::InputEvents::CMouseButtonReleasedEvent& e) -> bool {
        return true;
        });
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseButtonPressedEvent>([this](Engine::Events::InputEvents::CMouseButtonPressedEvent& e) -> bool {
        return true;
        });

    dispatcher.DispatchEvent<Engine::Networking::Events::CNetworkClientConnectedEvent>([this](Engine::Networking::Events::CNetworkClientConnectedEvent& e) -> bool {
        if (e.GetConnectionSocket())
        {
            LOG(LogTicTacToe, Success, "Client Connected!");
            if (CTicTacToe::Get().IsHost())
            {
                CTicTacToe::Get().SubmitToMainThread([this]() {
                    // push first to prevent the layer stack being empty
                    /// TODO: HAVE AN EMPTY LAYER ALWAYS
                    CTicTacToe::Get().PushLayer(new CBoardLayer());
                    CTicTacToe::Get().PopOverlay(this);
                    });
                return true;
            }
            return false;
        }
        return false;
        });

    dispatcher.DispatchEvent<Engine::Networking::Events::CNetworkPacketReceivedEvent>([this](Engine::Networking::Events::CNetworkPacketReceivedEvent& e) -> bool {
        if (!CTicTacToe::Get().IsHost())
        {
            CTicTacToe::Get().SubmitToMainThread([this]() {
                // push first to prevent the layer stack being empty
                /// TODO: HAVE AN EMPTY LAYER ALWAYS
                CTicTacToe::Get().PushLayer(new CBoardLayer());
                CTicTacToe::Get().PopOverlay(this);
                });
        }
        return true;
        });
}


}   // namespace Game