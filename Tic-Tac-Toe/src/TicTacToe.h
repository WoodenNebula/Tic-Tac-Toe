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

    virtual void OnEvent(Engine::Events::CEventBase& event) override;

    void MakeMove(const SCellPosition& Position);
    EGameState GetCurrentGameState();

    inline ECellState GetCurrentPlayer() const { return m_CurrentPlayer; }
    const inline std::vector<std::vector<ECellState>>& GetBoard() const { return m_Board; }

    Point2D<float> GetNDCFromViewport(const Point2D<double>& ViewportCoords);

    bool Net_OnGameStateReceived(CNetPayloadReceivedEvent& event);
    bool Net_ReplicateGameState() const;

    SNetPayload Net_PackGameState() const;
    std::tuple < ECellState, BoardState> Net_UnpackGameState(const SNetPayload& NetPayload) const;

    void OnGameStateChanged(EGameState newState);

    inline bool IsHost() const { return m_RequestedNetworkRole == Engine::Networking::ENetworkRole::Host; }
    inline bool IsWaitingForOtherPlayer() const { return  GetCurrentPlayer() != (IsHost() ? ECellState::X : ECellState::O); }

private:
    ECellState m_CurrentPlayer{ X };
    BoardState m_Board{
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
};
}
