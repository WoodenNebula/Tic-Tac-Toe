#include <Engine.h>
#include "Core/EntryPoint.h"

DECLARE_LOG_CATEGORY(TicTacToe)

class TicTacToeLayer : public Engine::Layer
{
public:
    TicTacToeLayer() : Layer("TicTacToeLayer") {}

    virtual ~TicTacToeLayer() = default;

    virtual void OnAttach() override
    {
        LOG(TicTacToe, TRACE, "Layer Attached in {}", m_Name);
    }
    virtual void OnDetach() override
    {
        LOG(TicTacToe, TRACE, "Layer DeAttached in {}", m_Name);
    }
    virtual void OnUpdate(float deltaTime) override
    {
        // Update logic for the Tic-Tac-Toe layer
        //LOG(TicTacToe, TRACE, "Layer Updated in {} with deltaTime {}", m_Name, deltaTime);
    }
    virtual void OnEvent(const Engine::Events::EventBase& event) override
    {
        // Event handling logic for the Tic-Tac-Toe layer
        LOG(TicTacToe, TRACE, "Event Received in {}", m_Name);
    }
};

class TicTacToe : public Engine::Application
{
public:
    TicTacToe(const Engine::SApplicationProps& appProps) : Engine::Application(appProps)
    {
        m_LayerStack.PushLayer(new TicTacToeLayer());
    }
};

Engine::Application* Engine::CreateApplication(int argc, char* argv[])
{
    Engine::SWindowProps windowProps = {
        {800, 600},
        {100, 100},
        "Tic-Tac-Toe"
    };
    Engine::SApplicationProps appProps = { windowProps };
    return new TicTacToe(appProps);
}