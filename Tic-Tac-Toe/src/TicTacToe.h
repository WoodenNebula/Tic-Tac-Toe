#pragma once

#include <Engine.h>

#include <filesystem>
#include <string>
#include <vector>

#include "TicTacToeLayers.h"

DECLARE_LOG_CATEGORY_LEVEL(TicTacToe, INFO)
namespace Game
{

enum ECellState
{
    EMPTY,
    X,
    O
};

enum EGameState
{
    ONGOING,
    DRAW,
    X_WINS,
    O_WINS
};

struct SCellPosition
{
    int Row;
    int Col;
    bool IsValid() const { return Row >= 0 && Row < 3 && Col >= 0 && Col < 3; }
};


class TicTacToe : public Engine::Application
{
public:
    static TicTacToe& Get();
    TicTacToe(const Engine::SApplicationProps& appProps);

    virtual Engine::SGenericError Init() override;

    void MakeMove(const SCellPosition& Position);
    EGameState GetCurrentGameState();

    void OnEvent(Engine::Events::EventBase& event) override;

    inline ECellState GetCurrentPlayer() const { return m_CurrentPlayer; }
    const inline std::vector<std::vector<ECellState>>& GetBoard() const { return m_Board; }

    Engine::Point2D<float> GetNDCFromViewport(const Engine::Point2D<double>& ViewportCoords);

private:
    ECellState m_CurrentPlayer{ X };
    std::vector<std::vector<ECellState>> m_Board{
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
};
}