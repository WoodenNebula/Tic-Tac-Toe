#include "Engine.h"
#include "Core/EntryPoint.h"

#include "TicTacToe.h"

Engine::CApplication* Engine::CreateApplication(int argc, char* argv[])
{
    Engine::SWindowProps windowProps = {
        .Dimension = {800, 600},
        .Position = {100, 100},
        .Title = "Tic-Tac-Toe",
        .WindowIconPath = "./res/textures/Tic-Tac-Toe.png"
    };
    Engine::SApplicationProps appProps = { windowProps };
    Engine::CApplication::App = new Game::CTicTacToe(appProps);
    return Engine::CApplication::App;
}