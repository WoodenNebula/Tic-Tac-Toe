#include "Engine.h"
#include "Core/EntryPoint.h"

#include "TicTacToe.h"

Engine::Application* Engine::CreateApplication(int argc, char* argv[])
{
    Engine::SWindowProps windowProps = {
        .Dimension = {800, 600},
        .Position = {100, 100},
        .Title = "Tic-Tac-Toe",
        .WindowIconPath = "./res/textures/Tic-Tac-Toe.png"
    };
    Engine::SApplicationProps appProps = { windowProps };
    Engine::Application::App = new Game::TicTacToe(appProps);
    return Engine::Application::App;
}