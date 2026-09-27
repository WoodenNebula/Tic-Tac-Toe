#include "Engine.h"
#include "Core/Util/CommandLine.h"
#include "Core/EntryPoint.h"

#include "TicTacToe.h"

#include <string>


Engine::Networking::ENetworkRole PromptHostOrJoin()
{
    using namespace Engine::Networking;

    while (true)
    {
        Print("Host(h) or Join(j) room?");
        char c = 0;
        std::cin >> c;

        if (c == 'h')
            return ENetworkRole::Host;
        else if (c == 'j')
            return ENetworkRole::Client;
        else
            Print("{}====\nPlease enter a valid option\n===={}", COLOR_YELLOW, COLOR_RESET);
    }

    return ENetworkRole::None;
}


Engine::CApplication* Engine::CreateApplication(int argc, char* argv[])
{

    auto [NetworkState, sockAddress] = Engine::SAppCmdLineArgs::ParseArgs(argc, argv);

    switch (NetworkState)
    {
    case Engine::Networking::ENetworkRole::None:
    default:
        NetworkState = PromptHostOrJoin();
        break;
    case Engine::Networking::ENetworkRole::Client:
    case Engine::Networking::ENetworkRole::Host:
        break;
    }

    std::string Username = Engine::ToString(NetworkState);

    std::string Title = "Tic-Tac-Toe (" + Engine::ToString(NetworkState) + ")";

    Engine::SWindowProps windowProps = {
        .Dimension = {800, 600},
        .Position = {100, 100},
        .Title = Title,
        .WindowIconPath = "./res/textures/Tic-Tac-Toe.png"
    };
    Engine::SApplicationProps appProps = { windowProps, Username };
    Engine::CNetworkedApplication::App = new Game::CTicTacToe(appProps, NetworkState);

    return Engine::CNetworkedApplication::App;
}