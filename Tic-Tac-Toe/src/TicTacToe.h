#pragma once

#include <Engine.h>

#include <filesystem>
#include <string>
#include <vector>

#include "TicTacToeTypes.h"
#include "TicTacToeLayers.h"

DECLARE_LOG_CATEGORY_LEVEL(TicTacToe, Info)
namespace Game
{

struct SCellPosition
{
    int Row;
    int Col;
    bool IsValid() const { return Row >= 0 && Row < 3 && Col >= 0 && Col < 3; }
};


class CTicTacToe : public Engine::CNetworkedApplication
{
public:
    static CTicTacToe& Get();
    static void Reset();
    CTicTacToe(const Engine::SApplicationProps& appProps, Engine::Networking::ENetworkRole initNetRole);
    ~CTicTacToe() {}

    virtual Engine::SGenericError Init() override;

    void MakeMove(const SCellPosition& Position);
    EGameState GetCurrentGameState();

    void OnEvent(Engine::Events::CEventBase& event) override;

    inline ECellState GetCurrentPlayer() const { return m_CurrentPlayer; }
    const inline std::vector<std::vector<ECellState>>& GetBoard() const { return m_Board; }

    Point2D<float> GetNDCFromViewport(const Point2D<double>& ViewportCoords);

private:
    ECellState m_CurrentPlayer{ X };
    std::vector<std::vector<ECellState>> m_Board{
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
};
}
