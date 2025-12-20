#include "Application/Application.h"
#include "Error/Error.h"

#include <iostream>

int main(int argc, char *argv[])
{
    Engine::SWindowProps WindowProps = {
        {800, 600},
        {0, 0},
        "Tic-Tac-Toe"};

    Engine::SApplicationProps AppProps = {WindowProps};

    Engine::Application App(AppProps);

    std::cout << "App Created\n";

    Engine::SGenericError Error = App.Init();
    std::cout << "App Init\n";

    if (Error)
    {
        std::cerr << Error;
        return Error.ErrorCode;
    }

    App.Run();
    App.Shutdown();

    return 0;
}
