#include <Engine.h>
#include <filesystem>
#include "TicTacToeLayers.h"

namespace Game
{

void BoardLayer::OnAttach()
{
    LOG(TicTacToe, TRACE, "Layer Attached in {}", m_Name);
    std::filesystem::path vp("res/shader/vertex/basicShader.vertexShader");
    std::filesystem::path fp("res/shader/fragment/default.fragmentShader");

    m_Shader = std::make_unique<Engine::Shader>(vp, fp);
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

    //const float lineWidth = 0.02f;
    //const float gridSize = 0.8f;    // Total grid size in NDC (-0.8 to 0.8)
    //const float cellSize = gridSize / 3.0f;

    //// Vertical line 1 (left)
    //float verticalLine1[] = {
    //    -cellSize - lineWidth / 2, -gridSize / 2, 0.0f,  // Bottom-left
    //    -cellSize + lineWidth / 2, -gridSize / 2, 0.0f,  // Bottom-right
    //    -cellSize - lineWidth / 2,  gridSize / 2, 0.0f   // Top-left
    //    - cellSize + lineWidth / 2,  gridSize / 2, 0.0f,  // Top-right
    //};

    //// Vertical line 2 (right)
    //float verticalLine2[] = {
    //    cellSize - lineWidth / 2, -gridSize / 2, 0.0f,  // Bottom-left
    //    cellSize + lineWidth / 2, -gridSize / 2, 0.0f,  // Bottom-right
    //    cellSize + lineWidth / 2,  gridSize / 2, 0.0f,  // Top-right
    //    cellSize - lineWidth / 2,  gridSize / 2, 0.0f   // Top-left
    //};

    //// Horizontal line 1 (top)
    //float horizontalLine1[] = {
    //    -gridSize / 2, cellSize - lineWidth / 2, 0.0f,  // Bottom-left
    //     gridSize / 2, cellSize - lineWidth / 2, 0.0f,  // Bottom-right
    //     gridSize / 2, cellSize + lineWidth / 2, 0.0f,  // Top-right
    //    -gridSize / 2, cellSize + lineWidth / 2, 0.0f   // Top-left
    //};

    //// Horizontal line 2 (bottom)
    //float horizontalLine2[] = {
    //    -gridSize / 2, -cellSize - lineWidth / 2, 0.0f,  // Bottom-left
    //     gridSize / 2, -cellSize - lineWidth / 2, 0.0f,  // Bottom-right
    //     gridSize / 2, -cellSize + lineWidth / 2, 0.0f,  // Top-right
    //    -gridSize / 2, -cellSize + lineWidth / 2, 0.0f   // Top-left
    //};

    //// Indices for each rectangle (2 triangles)
    //unsigned int indices[] = {
    //    0, 1, 2,  // First triangle
    //    2, 3, 0   // Second triangle
    //};

    //glm::mat4 mvp = glm::mat4(1.0f);
    //glm::vec4 lineColor(1.0f, 1.0f, 1.0f, 1.0f);


    //m_Shader->Bind();
    //m_Shader->SetUniformMat4f("u_MVP", mvp);
    //m_Shader->SetUniform4f("u_Color", lineColor.r, lineColor.g, lineColor.b, lineColor.a);


    //// Draw vertical line 1
    //Engine::VertexBuffer vb1(verticalLine1, sizeof(verticalLine1));
    //Engine::IndexBuffer ib1(indices, 6);
    //Engine::VertexArray va1(6);

    //Engine::Renderer::DrawLine(va1, 6);
    ////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    ////Engine::Renderer::DrawLine()
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //// Draw vertical line 2
    //Engine::VertexBuffer vb2(verticalLine2, sizeof(verticalLine2));
    //Engine::Renderer::DrawLine(va1, 6);

    ////glEnableVertexAttribArray(0);
    ////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //// Draw horizontal line 1
    //Engine::VertexBuffer vb3(horizontalLine1, sizeof(horizontalLine1));
    //Engine::Renderer::DrawLine(va1, 6);

    ////glEnableVertexAttribArray(0);
    ////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //// Draw horizontal line 2
    //Engine::VertexBuffer vb4(horizontalLine2, sizeof(horizontalLine2));
    //Engine::Renderer::DrawLine(va1, 6);

    ////glEnableVertexAttribArray(0);
    ////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    ////glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

}