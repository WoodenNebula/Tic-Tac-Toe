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
protected:
    std::unique_ptr<Engine::Shader> m_Shader{ nullptr };
};
}