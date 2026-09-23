#include "TicTacToe.h"

#include <filesystem>
#include <string>
#include <vector>

#include "TicTacToeTypes.h"

namespace Game
{
DECLARE_LOG_CATEGORY_LEVEL(TicTacToe, Info)

CTicTacToe& CTicTacToe::Get()
{
    return static_cast<CTicTacToe&>(*Engine::CApplication::App);
}

void CTicTacToe::Reset()
{
    auto& app = CTicTacToe::Get();
    app.m_CurrentPlayer = (app.m_CurrentPlayer == X) ? O : X;
    app.m_Board = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
}

CTicTacToe::CTicTacToe(const Engine::SApplicationProps& appProps) : Engine::CApplication(appProps)
{
}

Engine::SGenericError CTicTacToe::Init()
{
    Engine::SGenericError  err = CApplication::Init();
    PushLayer(new CBoardLayer());
    return err;
}

void CTicTacToe::OnEvent(Engine::Events::CEventBase& event)
{
    LOG(LogTicTacToe, Trace, "Event {} Received in TicTacToe Application", event);
    Engine::Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CKeyPressedEvent>([this](Engine::Events::InputEvents::CKeyPressedEvent& e) -> bool {
        if (e.GetKey() == Engine::Events::EKey::Escape)
        {
            Engine::Events::CWindowCloseEvent windowCloseEvent;
            OnWindowCloseEvent(windowCloseEvent);
            return true;
        }
        return false;
        });

    CApplication::OnEvent(event);
}

void CTicTacToe::MakeMove(const SCellPosition& Position)
{
    auto [row, col] = Position;
    if (!Position.IsValid())
    {
        LOG(LogTicTacToe, Warning, "Invalid move: ({}, {})", row, col);
        return;
    }
    if (m_Board[row][col] != EMPTY)
    {
        LOG(LogTicTacToe, Warning, "Cell already occupied: ({}, {}) = {}", row, col, (int)m_Board[row][col]);
        return;
    }
    m_Board[row][col] = m_CurrentPlayer;
    m_CurrentPlayer = (m_CurrentPlayer == X) ? O : X;
}

EGameState CTicTacToe::GetCurrentGameState()
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

Engine::Point2D<float> CTicTacToe::GetNDCFromViewport(const Engine::Point2D<double>& ViewportCoords)
{
    auto Viewport = m_ApplicationProps.WindowProps.Dimension;
    Engine::Point2D<float> ndc;
    // X: [0, width] -> [-1, 1]
    ndc.x = (float)((2.0 * ViewportCoords.x) / Viewport.x - 1.0);

    // Y: [0, height] -> [1, -1]  (flip Y)
    ndc.y = (float)(1.0 - (2.0 * ViewportCoords.y) / Viewport.y);

    return ndc;
}


}
