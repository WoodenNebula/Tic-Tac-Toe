#include "TicTacToe.h"

#include <filesystem>
#include <string>
#include <vector>



namespace Game
{
DECLARE_LOG_CATEGORY_LEVEL(TicTacToe, INFO)

TicTacToe& TicTacToe::Get()
{
    return static_cast<TicTacToe&>(*Engine::Application::App);
}

TicTacToe::TicTacToe(const Engine::SApplicationProps& appProps) : Engine::Application(appProps)
{
    m_LayerStack.PushLayer(new TicTacToeLayer());
    m_LayerStack.PushLayer(new BoardLayer());

}

void TicTacToe::OnEvent(Engine::Events::EventBase& event)
{
    LOG(TicTacToe, TRACE, "Event {} Received in TicTacToe Application", event);
    Engine::Events::EventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::KeyPressedEvent>([this](Engine::Events::InputEvents::KeyPressedEvent& e) -> bool {
        if (e.GetKey() == Engine::Events::Key::Escape)
        {
            Engine::Events::WindowCloseEvent windowCloseEvent;
            OnWindowCloseEvent(windowCloseEvent);
            return true;
        }
        return false;
        });

}

void TicTacToe::MakeMove(int row, int col)
{
    if (row < 0 || row >= 3 || col < 0 || col >= 3)
    {
        LOG(TicTacToe, WARN, "Invalid move: ({}, {})", row, col);
        return;
    }
    if (m_Board[row][col] != EMPTY)
    {
        LOG(TicTacToe, WARN, "Cell already occupied: ({}, {}) = {}", row, col, (int)m_Board[row][col]);
        return;
    }
    m_Board[row][col] = m_CurrentPlayer;
    m_CurrentPlayer = (m_CurrentPlayer == X) ? O : X;
}

EGameState TicTacToe::GetCurrentGameState()
{
    // Check rows and columns
    for (int i = 0; i < 3; ++i)
    {
        if (m_Board[i][0] != EMPTY && m_Board[i][0] == m_Board[i][1] && m_Board[i][1] == m_Board[i][2])
        {
            return (m_Board[i][0] == X) ? X_WINS : O_WINS;
        }
        if (m_Board[0][i] != EMPTY && m_Board[0][i] == m_Board[1][i] && m_Board[1][i] == m_Board[2][i])
        {
            return (m_Board[0][i] == X) ? X_WINS : O_WINS;
        }
    }
    // Check diagonals
    if (m_Board[0][0] != EMPTY && m_Board[0][0] == m_Board[1][1] && m_Board[1][1] == m_Board[2][2])
    {
        return (m_Board[0][0] == X) ? X_WINS : O_WINS;
    }
    if (m_Board[0][2] != EMPTY && m_Board[0][2] == m_Board[1][1] && m_Board[1][1] == m_Board[2][0])
    {
        return (m_Board[0][2] == X) ? X_WINS : O_WINS;
    }
    // Check for draw or ongoing
    for (const auto& row : m_Board)
    {
        for (const auto& cell : row)
        {
            if (cell == EMPTY)
            {
                return ONGOING;
            }
        }
    }
    return DRAW;
}

}