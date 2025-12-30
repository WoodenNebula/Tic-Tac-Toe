#include <Engine.h>
#include <filesystem>
#include "TicTacToeLayers.h"

namespace Game
{


void BoardLayer::OnAttach()
{
    LOG(TicTacToe, TRACE, "Layer Attached in {}", m_Name);
    m_Grid = {
        .H1 = {
            .Start = {  -m_GridSize,     m_CellSize,    0.0f},
            .End = {     m_GridSize,     m_CellSize,    0.0f}
        },
        .H2 = {
            .Start = {  -m_GridSize,    -m_CellSize,    0.0f},
            .End = {     m_GridSize,    -m_CellSize,    0.0f}
        },
        .V1 = {
            .Start = {  -m_CellSize,     m_GridSize,    0.0f},
            .End = {    -m_CellSize,    -m_GridSize,    0.0f}
        },
        .V2 = {
            .Start = {   m_CellSize,     m_GridSize,    0.0f},
            .End = {     m_CellSize,    -m_GridSize,    0.0f}
        },
        .LineWidth = 5.0f,
        .Color = {1.0f, 0.5f, 1.0f, 1.0f}
    };
}

void BoardLayer::OnDetach()
{
    LOG(TicTacToe, TRACE, "Layer DeAttached in {}", m_Name);
}

void BoardLayer::OnUpdate(float deltaTime)
{
    Engine::Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f });
    Engine::Renderer::Clear();

    DrawBoard();
}

void BoardLayer::OnEvent(Engine::Events::EventBase& event)
{
    // Event handling logic for the Tic-Tac-Toe layer
    LOG(TicTacToe, TRACE, "Event {} Received in {}", event, m_Name);

    Engine::Events::EventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::MouseMovedEvent>(BIND_EVENT_CB(BoardLayer::OnMouseMoved));
    dispatcher.DispatchEvent<Engine::Events::InputEvents::MouseButtonPressedEvent>(BIND_EVENT_CB(BoardLayer::OnMouseButtonPressed));
}

bool BoardLayer::OnMouseMoved(Engine::Events::InputEvents::MouseMovedEvent& event)
{
    m_MousePosition = event.GetPos();
    return true;
}


bool BoardLayer::OnMouseButtonPressed(Engine::Events::InputEvents::MouseButtonPressedEvent& event)
{
    auto pos = TicTacToe::Get().GetNDCFromViewport(m_MousePosition);
    SCellPosition cellPos = NDCToCellPosition(pos);
    LOG(TicTacToe, INFO, "Mouse Button Pressed at Position: {} -> cell {}, {}", pos.ToString(), cellPos.Row, cellPos.Col);

    if (cellPos.IsValid())
    {
        TicTacToe::Get().MakeMove(cellPos);
    }
    return true;
}

void BoardLayer::DrawBoard()
{
    auto board = TicTacToe::Get().GetBoard();

    DrawGrid();
    for (const auto& row : board)
    {
        for (const auto& cell : row)
        {
            DrawCell(&cell - &row[0], &row - &board[0], cell);
        }
    }
}

void BoardLayer::DrawGrid()
{
    // Drawing logic for the grid lines

    LOG(TicTacToe, TRACE, "Drawing Grid Lines");

    Engine::Renderer::StartDraw();

    Engine::Renderer::DrawLine(m_Grid.H1.Start, m_Grid.H1.End, m_Grid.Color, m_Grid.LineWidth);
    Engine::Renderer::DrawLine(m_Grid.H2.Start, m_Grid.H2.End, m_Grid.Color, m_Grid.LineWidth);

    Engine::Renderer::DrawLine(m_Grid.V1.Start, m_Grid.V1.End, m_Grid.Color, m_Grid.LineWidth);
    Engine::Renderer::DrawLine(m_Grid.V2.Start, m_Grid.V2.End, m_Grid.Color, m_Grid.LineWidth);

    Engine::Renderer::Flush();
}

void BoardLayer::DrawCell(int row, int col, enum ECellState state)
{
    switch (state)
    {
    case X: DrawX(row, col); break;
    case O: DrawO(row, col); break;
    case EMPTY: break;
    }
}

void BoardLayer::DrawX(int row, int col)
{
    // Drawing logic for X
}
void BoardLayer::DrawO(int row, int col)
{
    // Drawing logic for O
}

SCellPosition BoardLayer::NDCToCellPosition(const Engine::Point2D<float>& NDCPos) const
{
    SCellPosition cellPos{ -1, -1 };
    float halfGridSize = m_GridSize / 2.0f;
    bool xInGrid = NDCPos.x > -m_GridSize && NDCPos.x < m_GridSize;
    bool yInGrid = NDCPos.y > -m_GridSize && NDCPos.y < m_GridSize;

    if (!xInGrid)
    {
        LOG(TicTacToe, WARN, "X Position {} out of grid bounds Grid, Cell = ({}, {})", NDCPos.x, m_GridSize, m_CellSize);
        return cellPos;
    }
    if (!yInGrid)
    {
        LOG(TicTacToe, WARN, "Y Position {} out of grid bounds Grid, Cell = ({}, {})", NDCPos.y, m_GridSize, m_CellSize);
        return cellPos;
    }

    if (NDCPos.x > -m_GridSize && NDCPos.x <= -m_CellSize)
    {
        cellPos.Col = 0;
    }
    else if (NDCPos.x > -m_CellSize && NDCPos.x <= m_CellSize)
    {
        cellPos.Col = 1;
    }
    else
    {
        cellPos.Col = 2;
    }

    if (NDCPos.y < m_GridSize && NDCPos.y >= m_CellSize)
    {
        cellPos.Row = 0;
    }
    else if (NDCPos.y < m_CellSize && NDCPos.y >= -m_CellSize)
    {
        cellPos.Row = 1;
    }
    else
    {
        cellPos.Row = 2;
    }

    return cellPos;
}


}