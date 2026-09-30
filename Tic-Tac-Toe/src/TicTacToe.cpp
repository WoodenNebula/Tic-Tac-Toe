#include "TicTacToe.h"

#include <filesystem>
#include <string>
#include <vector>

#include "TicTacToeTypes.h"

namespace Game
{
DECLARE_LOG_CATEGORY_LEVEL(TicTacToe, Info)

CTicTacToe& CTicTacToe::Get()
{
    return static_cast<CTicTacToe&>(*Engine::CApplication::App);
}

void CTicTacToe::Reset()
{
    auto& app = CTicTacToe::Get();
    app.m_CurrentPlayer = (app.m_CurrentPlayer == X) ? O : X;
    app.m_Board = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
}

CTicTacToe::CTicTacToe(const Engine::SApplicationProps& appProps, Engine::Networking::ENetworkRole initNetRole)
    : Engine::CNetworkedApplication(appProps, initNetRole)
{
}

Engine::SGenericError CTicTacToe::Init()
{
    using namespace Engine::Networking;
    Engine::SGenericError err = CNetworkedApplication::Init();
    if (!err)
    {
        ENetworkRole netRole = CNetworkSubsystem::Get().GetCurrentNetworkRole();
        ENetworkConnectionStatus netStatus = CNetworkSubsystem::Get().GetConnectionStatus();

        // start game if network setup is done
        if (netRole != ENetworkRole::None)
        {
            if (netStatus == ENetworkConnectionStatus::WaitingForConnection)
                PushLayer(new CWaitingForNetwork_OverlayLayer());
            else
                PushLayer(new CBoardLayer());
        }
        // We are in offline mode
        else
            PushLayer(new CBoardLayer());
    }
    return err;
}

void CTicTacToe::OnEvent(Engine::Events::CEventBase& event)
{
    LOG(LogTicTacToe, Trace, "Event {} Received in TicTacToe Application", event);
    Engine::Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Engine::Events::InputEvents::CKeyPressedEvent>([this](Engine::Events::InputEvents::CKeyPressedEvent& e) -> bool {
        if (e.GetKey() == Engine::Events::EKey::Escape)
        {
            Engine::Events::CWindowCloseEvent windowCloseEvent;
            OnWindowCloseEvent(windowCloseEvent);
            return true;
        }
        return false;
        });

    dispatcher.DispatchEvent<CNetPayloadReceivedEvent>(BIND_EVENT_CB(CTicTacToe::Net_OnGameStateReceived));

    Engine::CNetworkedApplication::OnEvent(event);
}

void CTicTacToe::MakeMove(const SCellPosition& Position)
{
    auto [row, col] = Position;
    if (!Position.IsValid())
    {
        LOG(LogTicTacToe, Warning, "Invalid move: ({}, {})", row, col);
        return;
    }
    if (m_Board[row][col] != EMPTY)
    {
        LOG(LogTicTacToe, Warning, "Cell already occupied: ({}, {}) = {}", row, col, (int)m_Board[row][col]);
        return;
    }
    m_Board[row][col] = m_CurrentPlayer;
    m_CurrentPlayer = (m_CurrentPlayer == X) ? O : X;
}

EGameState CTicTacToe::GetCurrentGameState()
{
    // Check rows and columns
    for (int i = 0; i < 3; ++i)
    {
        if (m_Board[i][0] != EMPTY && m_Board[i][0] == m_Board[i][1] && m_Board[i][1] == m_Board[i][2])
        {
            return (m_Board[i][0] == X) ? X_WINS : O_WINS;
        }
        if (m_Board[0][i] != EMPTY && m_Board[0][i] == m_Board[1][i] && m_Board[1][i] == m_Board[2][i])
        {
            return (m_Board[0][i] == X) ? X_WINS : O_WINS;
        }
    }
    // Check diagonals
    if (m_Board[0][0] != EMPTY && m_Board[0][0] == m_Board[1][1] && m_Board[1][1] == m_Board[2][2])
    {
        return (m_Board[0][0] == X) ? X_WINS : O_WINS;
    }
    if (m_Board[0][2] != EMPTY && m_Board[0][2] == m_Board[1][1] && m_Board[1][1] == m_Board[2][0])
    {
        return (m_Board[0][2] == X) ? X_WINS : O_WINS;
    }
    // Check for draw or ongoing
    for (const auto& row : m_Board)
    {
        for (const auto& cell : row)
        {
            if (cell == EMPTY)
            {
                return ONGOING;
            }
        }
    }
    return DRAW;
}

Point2D<float> CTicTacToe::GetNDCFromViewport(const Point2D<double>& ViewportCoords)
{
    Point2D<uint32_t> ViewportSize = GetWindowDimensions();
    Point2D<float> ndc;
     // X: [0, width] -> [-1, 1]
    ndc.x = (float)((2.0 * ViewportCoords.x) / ViewportSize.x - 1.0);

    // Y: [0, height] -> [1, -1]  (flip Y)
    ndc.y = (float)(1.0 - (2.0 * ViewportCoords.y) / ViewportSize.y);

    return ndc;
}

bool CTicTacToe::Net_OnGameStateReceived(CNetPayloadReceivedEvent& event)
{
    auto [currentPlayer, receivedBoardState] = Net_UnpackGameState(event.GetPayload());

    LOG(LogTicTacToe, Success, "Game state received over net");
    return true;
}

bool CTicTacToe::Net_ReplicateGameState() const
{
    auto payload = Net_PackGameState();
    auto& NetSys = Engine::Networking::CNetworkSubsystem::Get();

    NetSys.SendPayload(payload);

    LOG(LogTicTacToe, Info, "Game state sent over net");
    return true;
}

SNetPayload CTicTacToe::Net_PackGameState() const
{
    std::string strPayload = "";

    strPayload += ToString(m_CurrentPlayer);

    for (const auto& row : m_Board)
    {
        for (const auto& cell : row)
        {
            strPayload += ToString(cell);
        }
    }

    LOG(LogTicTacToe, Success, "Packed payload: {}", strPayload);

    SNetPayload payload{ strPayload };

    return payload;
}


std::tuple <ECellState, BoardState> CTicTacToe::Net_UnpackGameState(const SNetPayload& NetPayload) const
{
    std::string strPayload = NetPayload.BufferAsString();
    if (strPayload.size() < 1 + 3 * 3)
    {
        LOG(LogTicTacToe, Error, "MALFORMED PAYLOAD RECEIVED: {}", strPayload);
        return { m_CurrentPlayer, m_Board };
    }

    ECellState newCurrPlayer = FromString(strPayload.at(0));
    BoardState newBoard;

    for (size_t i = 0; i <= 2; i++)
    {
        newBoard[i] = {
            FromString(strPayload.at(i * 3 + 0)),
            FromString(strPayload.at(i * 3 + 1)),
            FromString(strPayload.at(i * 3 + 2)),
        };
    }
    LOG(LogTicTacToe, Success, "Unpacked payload: {} -> {}, {}", strPayload, ToString(newCurrPlayer), ToString(newBoard));

    return { newCurrPlayer, newBoard };
}

}   // namespace Game
