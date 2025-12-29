#pragma once
#include <Engine.h>
#include "TicTacToe.h"

namespace Game
{

class TicTacToeLayer : public Engine::Layer
{
public:
    TicTacToeLayer() : Layer("TicTacToeLayer") {}

    virtual ~TicTacToeLayer() = default;

    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(Engine::Events::EventBase& event) override;
};


class BoardLayer : public Engine::Layer
{
    struct SGrid
    {
        struct SLine
        {
            glm::vec3 Start;
            glm::vec3 End;
        } H1, H2, V1, V2;

        float LineWidth{ 2.0f };
        glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
    };

public:
    BoardLayer() : Layer("BoardLayer") {}
    virtual ~BoardLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float deltaTime) override;
    virtual void OnEvent(Engine::Events::EventBase& event) override;
private:
    void DrawBoard();
    void DrawGrid();
    void DrawCell(int row, int col, enum ECellState state);
    void DrawX(int row, int col);
    void DrawO(int row, int col);

private:
    const float m_GridSize = 0.7f;
    const float m_CellSize = m_GridSize / 3.0f;
    SGrid m_Grid;
};
} // namespace Game