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


class TicTacToe : public Engine::Application
{
public:
    static TicTacToe& Get();
    TicTacToe(const Engine::SApplicationProps& appProps);

    void MakeMove(int row, int col);
    EGameState GetCurrentGameState();

    void OnEvent(Engine::Events::EventBase& event) override;

    inline ECellState GetCurrentPlayer() const { return m_CurrentPlayer; }
    const inline std::vector<std::vector<ECellState>>& GetBoard() const { return m_Board; }

private:
    ECellState m_CurrentPlayer{ X };
    std::vector<std::vector<ECellState>> m_Board{
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
};
}