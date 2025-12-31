#include <Engine.h>
#include "TicTacToeLayers.h"

namespace Game
{
void TicTacToeLayer::OnAttach()
{
    LOG(TicTacToe, TRACE, "Layer Attached in {}", m_Name);

    m_XTexture = std::make_shared<Engine::CTexture>("./res/textures/X.png");
    m_OTexture = std::make_shared<Engine::CTexture>("./res/textures/O.png");
}


void TicTacToeLayer::OnUpdate(float deltaTime)
{
    switch (TicTacToe::Get().GetCurrentGameState())
    {
    case ONGOING:
        TicTacToe::Get().SubmitToMainThread([this]() {
            TicTacToe::Get().PopOverlay(this);
            });
        break;
    case DRAW:
    {
        Engine::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.2f });
        Engine::Renderer::Clear();

        Engine::Renderer::StartDraw();
        Engine::Renderer::DrawLine({ -0.5f, -0.5f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 10.0f);
        Engine::Renderer::Flush();
        break;
    }
    case X_WINS:
    {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));

        Engine::Renderer::SetClearColor({ 0.2f, 0.0f, 0.0f, 0.2f });
        Engine::Renderer::Clear();
        Engine::Renderer::StartDraw();
        Engine::Renderer::DrawQuad(transform, m_XTexture);
        Engine::Renderer::Flush();
        break;
    }
    case O_WINS:
    {
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f * 0.8f, 0.3f * 0.8f, 1.0f));
        Engine::Renderer::SetClearColor({ 0.0f, 0.0f, 0.2f, 0.2f });
        Engine::Renderer::Clear();
        Engine::Renderer::StartDraw();
        Engine::Renderer::DrawQuad(transform, m_OTexture);
        Engine::Renderer::Flush();
        break;
    }
    }

    // Update logic for the Tic-Tac-Toe layer
    //LOG(TicTacToe, TRACE, "Layer Updated in {} with deltaTime {}", m_Name, deltaTime);
}
void TicTacToeLayer::OnEvent(Engine::Events::EventBase& event)
{
    // Event handling logic for the Tic-Tac-Toe layer
    LOG(TicTacToe, TRACE, "Event {} Received in {}", event, m_Name);

    Engine::Events::EventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::KeyPressedEvent>([this](Engine::Events::InputEvents::KeyPressedEvent& e) -> bool {
        if (e.GetKey() == Engine::Events::Key::R)
        {
            TicTacToe::Get().SubmitToMainThread([this]() {
                TicTacToe::Get().Reset();
                TicTacToe::Get().PopOverlay(this);
                });
            return true;
        }
        return false;
        });

}
}
