#include <Engine.h>
#include "TicTacToeLayers.h"

namespace Game
{

void TicTacToeLayer::OnUpdate(float deltaTime)
{
    Engine::Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f });
    Engine::Renderer::Clear();
    // Update logic for the Tic-Tac-Toe layer
    //LOG(TicTacToe, TRACE, "Layer Updated in {} with deltaTime {}", m_Name, deltaTime);
}
void TicTacToeLayer::OnEvent(Engine::Events::EventBase& event)
{
    // Event handling logic for the Tic-Tac-Toe layer
    LOG(TicTacToe, TRACE, "Event {} Received in {}", event, m_Name);

}
}
