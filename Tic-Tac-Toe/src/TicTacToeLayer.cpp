#include <Engine.h>
#include "TicTacToeTypes.h"
#include "TicTacToeLayers.h"

namespace Game
{
void CTicTacToeLayer::OnAttach()
{
    LOG(LogTicTacToe, Trace, "Layer Attached in {}", m_Name);

    m_XTexture = std::make_shared<Engine::CTexture>("./res/textures/X.png");
    m_OTexture = std::make_shared<Engine::CTexture>("./res/textures/O.png");
}


void CTicTacToeLayer::OnUpdate(float deltaTime)
{
    switch (CTicTacToe::Get().GetCurrentGameState())
    {
    case ONGOING:
        CTicTacToe::Get().SubmitToMainThread([this]() {
            CTicTacToe::Get().PopOverlay(this);
            });
        break;
    //case DRAW:
    //{
    //    Engine::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.2f });
    //    Engine::Renderer::Clear();

    //    Engine::Renderer::StartDraw();
    //    Engine::Renderer::DrawLine({ -0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 10.0f);
    //    Engine::Renderer::Flush();
    //    break;
    //}
    //case X_WINS:
    //{
    //    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    //    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));

    //    Engine::Renderer::SetClearColor({ 0.2f, 0.0f, 0.0f, 0.2f });
    //    Engine::Renderer::Clear();
    //    Engine::Renderer::StartDraw();
    //    Engine::Renderer::DrawQuad(transform, m_XTexture);
    //    Engine::Renderer::Flush();
    //    break;
    //}
    //case O_WINS:
    //{
    //    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    //    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));
    //    Engine::Renderer::SetClearColor({ 0.0f, 0.0f, 0.2f, 0.2f });
    //    Engine::Renderer::Clear();
    //    Engine::Renderer::StartDraw();
    //    Engine::Renderer::DrawQuad(transform, m_OTexture);
    //    Engine::Renderer::Flush();
    //    break;
    //}
    }

    // Update logic for the Tic-Tac-Toe layer
    //LOG(LogTicTacToe, TRACE, "Layer Updated in {} with deltaTime {}", m_Name, deltaTime);
}
void CTicTacToeLayer::OnEvent(Engine::Events::CEventBase& event)
{
    // Event handling logic for the Tic-Tac-Toe layer
    LOG(LogTicTacToe, Trace, "Event {} Received in {}", event, m_Name);

    Engine::Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseButtonReleasedEvent>([this](Engine::Events::InputEvents::CMouseButtonReleasedEvent& e) -> bool {
        return true;
        });
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseButtonPressedEvent>([this](Engine::Events::InputEvents::CMouseButtonPressedEvent& e) -> bool {
        return true;
        });

    dispatcher.DispatchEvent<Engine::Events::InputEvents::CKeyPressedEvent>([this](Engine::Events::InputEvents::CKeyPressedEvent& e) -> bool {
        if (e.GetKey() == Engine::Events::EKey::R)
        {
            CTicTacToe::Get().SubmitToMainThread([this]() {
                CTicTacToe::Get().Reset();
                CTicTacToe::Get().PopOverlay(this);
                });
            return true;
        }
        return false;
        });

}

void CTicTacToeOverlayLayer::OnAttach()
{
    CTicTacToeLayer::OnAttach();
    switch (CTicTacToe::Get().GetCurrentGameState())
    {
    case DRAW:
    {
        Engine::CRenderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        break;
    }
    case X_WINS:
    {
        Engine::CRenderer::SetClearColor({ 0.2f, 0.0f, 0.0f, 1.0f });
        break;
    }
    case O_WINS:
    {
        Engine::CRenderer::SetClearColor({ 0.0f, 0.0f, 0.2f, 1.0f });
        break;
    }
    }
    Engine::CRenderer::Clear();
}


void CTicTacToeOverlayLayer::OnUpdate(float deltaTime)
{
    switch (CTicTacToe::Get().GetCurrentGameState())
    {
    case DRAW:
    {
        Engine::CRenderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Engine::CRenderer::Clear();

        Engine::CRenderer::StartDraw();
        Engine::CRenderer::DrawLine({ -0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 10.0f);
        Engine::CRenderer::Flush();
        break;
    }
    case X_WINS:
    {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));

        Engine::CRenderer::SetClearColor({ 0.2f, 0.0f, 0.0f, 1.0f });
        Engine::CRenderer::Clear();
        Engine::CRenderer::StartDraw();
        Engine::CRenderer::DrawQuad(transform, m_XTexture);
        Engine::CRenderer::Flush();
        break;
    }
    case O_WINS:
    {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));
        Engine::CRenderer::SetClearColor({ 0.0f, 0.0f, 0.2f, 1.0f });
        Engine::CRenderer::Clear();
        Engine::CRenderer::StartDraw();
        Engine::CRenderer::DrawQuad(transform, m_OTexture);
        Engine::CRenderer::Flush();
        break;
    }
    }
}
}