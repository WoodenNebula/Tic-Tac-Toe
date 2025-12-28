#include <Engine.h>
#include "Core/EntryPoint.h"
#include "TicTacToe.h"

Engine::Application* Engine::CreateApplication(int argc, char* argv[])
{
    Engine::SWindowProps windowProps = {
        {800, 600},
        {100, 100},
        "Tic-Tac-Toe"
    };
    Engine::SApplicationProps appProps = { windowProps };
    Engine::Application::App = new Game::TicTacToe(appProps);
    return Engine::Application::App;
}