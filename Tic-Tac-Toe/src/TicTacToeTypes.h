#pragma once
#include "Engine.h"
#include "Networking/Sockets/Sockets.h"
#include "Networking/Events/NetworkEvents.h"

#include <string>
#include <vector>

namespace Game
{
enum ECellState { EMPTY, X, O };
enum EGameState { ONGOING, DRAW, X_WINS, O_WINS };

using SNetPayload = Engine::Sockets::SSocketPayload;
using BoardState = std::vector < std::vector<ECellState>>;
using CNetPayloadReceivedEvent = Engine::Networking::Events::CNetworkPacketReceivedEvent;

static std::string ToString(ECellState cell)
{
    switch (cell)
    {
    case Game::EMPTY: return " ";
    case Game::X: return "X";
    case Game::O: return "O";
    default: return " ";
    }
}

static ECellState FromString(char c)
{
    if (c == 'X') return Game::X;
    else if (c == 'O') return Game::O;
    else return Game::EMPTY;
}

static std::string ToString(const BoardState& board)
{
    std::string str;
    for (const auto& row : board)
    {
        for (const auto& cell : row)
        {
            str += ToString(cell) + ", ";
        }
        str += "\n";
    }
    return str;
}
} // namespace Game
