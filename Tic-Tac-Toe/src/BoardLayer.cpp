#include <Engine.h>
#include <filesystem>
#include "TicTacToeTypes.h"
#include "TicTacToeLayers.h"

namespace Game
{


void CBoardLayer::OnAttach()
{
    LOG(LogTicTacToe, Trace, "Layer Attached in {}", m_Name);

    m_XTexture = std::make_shared<Engine::CTexture>("./res/textures/X.png");
    m_OTexture = std::make_shared<Engine::CTexture>("./res/textures/O.png");

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

void CBoardLayer::OnDetach()
{
    LOG(LogTicTacToe, Trace, "Layer DeAttached in {}", m_Name);
}

void CBoardLayer::OnUpdate(float deltaTime)
{
    Engine::CRenderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Engine::CRenderer::Clear();

    DrawBoard();
    switch (CTicTacToe::Get().GetCurrentGameState())
    {
    case ONGOING:
        break;
    case DRAW:
    {
        Engine::CRenderer::SetClearColor({ 0.5f, 0.5f, 0.5f, 1.0f });
        Engine::CRenderer::Clear();
        DrawBoard();
        break;
    }
    }
}

void CBoardLayer::OnEvent(Engine::Events::CEventBase& event)
{
    // Event handling logic for the Tic-Tac-Toe layer
    LOG(LogTicTacToe, Trace, "Event {} Received in {}", event, m_Name);

    Engine::Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseMovedEvent>(BIND_EVENT_CB(CBoardLayer::OnMouseMoved));
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CMouseButtonPressedEvent>(BIND_EVENT_CB(CBoardLayer::OnMouseButtonPressed));
}

bool CBoardLayer::OnMouseMoved(Engine::Events::InputEvents::CMouseMovedEvent& event)
{
    m_MousePosition = event.GetPos();
    return true;
}


bool CBoardLayer::OnMouseButtonPressed(Engine::Events::InputEvents::CMouseButtonPressedEvent& event)
{
    auto pos = CTicTacToe::Get().GetNDCFromViewport(m_MousePosition);
    SCellPosition cellPos = NDCToCellPosition(pos);
    LOG(LogTicTacToe, Info, "Mouse Button Pressed at Position: {} -> cell {}, {}", pos.ToString(), cellPos.Row, cellPos.Col);

    if (cellPos.IsValid())
    {
        CTicTacToe::Get().MakeMove(cellPos);

        EGameState gameState = CTicTacToe::Get().GetCurrentGameState();

        if (gameState != EGameState::ONGOING)
        {
            // Defer overlay push until after event handling completes
            CTicTacToe::Get().SubmitToMainThread([gameState]() {
                CTicTacToe::Get().PushOverlay(new CTicTacToeOverlayLayer());
                LOG(LogTicTacToe, Info, "Game ended with state: {}", (int)gameState);
                });
        }

        CTicTacToe::Get().Net_ReplicateGameState();
    }
    return true;
}

void CBoardLayer::DrawBoard()
{
    auto board = CTicTacToe::Get().GetBoard();

    DrawGrid();
    for (const auto& row : board)
    {
        for (const auto& cell : row)
        {
            DrawCell(static_cast<int>(&cell - &row[0]), static_cast<int>(&row - &board[0]), cell);
        }
    }
}

void CBoardLayer::DrawGrid()
{
    // Drawing logic for the grid lines

    LOG(LogTicTacToe, Trace, "Drawing Grid Lines");

    Engine::CRenderer::StartDraw();

    Engine::CRenderer::DrawLine(m_Grid.H1.Start, m_Grid.H1.End, m_Grid.Color, m_Grid.LineWidth);
    Engine::CRenderer::DrawLine(m_Grid.H2.Start, m_Grid.H2.End, m_Grid.Color, m_Grid.LineWidth);

    Engine::CRenderer::DrawLine(m_Grid.V1.Start, m_Grid.V1.End, m_Grid.Color, m_Grid.LineWidth);
    Engine::CRenderer::DrawLine(m_Grid.V2.Start, m_Grid.V2.End, m_Grid.Color, m_Grid.LineWidth);

    Engine::CRenderer::Flush();
}

void CBoardLayer::DrawCell(int row, int col, enum ECellState state)
{
    switch (state)
    {
    case X: DrawX(row, col); break;
    case O: DrawO(row, col); break;
    case EMPTY: break;
    }
}

void CBoardLayer::DrawX(int row, int col)
{
    auto [x, y, z] = CellPositionToNDC({ row, col });
    glm::vec3 position{ x, y, z };
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(m_CellSize * 0.8f, m_CellSize * 0.8f, 1.0f));
    Engine::CRenderer::StartDraw();
    Engine::CRenderer::DrawQuad(transform, m_XTexture);
    Engine::CRenderer::Flush();

}
void CBoardLayer::DrawO(int row, int col)
{
    auto [x, y, z] = CellPositionToNDC({ row, col });
    glm::vec3 position{ x, y, z };
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(m_CellSize * 0.8f, m_CellSize * 0.8f, 1.0f));

    Engine::CRenderer::StartDraw();
    Engine::CRenderer::DrawQuad(transform, m_OTexture);
    Engine::CRenderer::Flush();
}

Point3D<float> CBoardLayer::CellPositionToNDC(const SCellPosition& cellPos) const
{
    Point3D<float> NDCPos{ 0.0f, 0.0f , 0.0f };
    float cellOffset = (2.0f * m_GridSize) / 3.0f;

    /// THIS IS FLIPPED CUZ VECTOR<VECTOR> AHHHH
    switch (cellPos.Col)
    {
    case 0: NDCPos.y = cellOffset; break;
    case 1: NDCPos.y = 0.0f; break;
    case 2: NDCPos.y = -cellOffset; break;
    }

    switch (cellPos.Row)
    {
    case 0: NDCPos.x = -cellOffset; break;
    case 1: NDCPos.x = 0.0f; break;
    case 2: NDCPos.x = cellOffset; break;
    }

    return NDCPos;
}

SCellPosition CBoardLayer::NDCToCellPosition(const Point2D<float>& NDCPos) const
{
    SCellPosition cellPos{ -1, -1 };
    float halfGridSize = m_GridSize / 2.0f;
    bool xInGrid = NDCPos.x > -m_GridSize && NDCPos.x < m_GridSize;
    bool yInGrid = NDCPos.y > -m_GridSize && NDCPos.y < m_GridSize;

    if (!xInGrid)
    {
        LOG(LogTicTacToe, Warning, "X Position {} out of grid bounds Grid, Cell = ({}, {})", NDCPos.x, m_GridSize, m_CellSize);
        return cellPos;
    }
    if (!yInGrid)
    {
        LOG(LogTicTacToe, Warning, "Y Position {} out of grid bounds Grid, Cell = ({}, {})", NDCPos.y, m_GridSize, m_CellSize);
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
