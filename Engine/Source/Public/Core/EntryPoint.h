#pragma once

#ifdef MAIN

#else
#define MAIN
/// TODO: Abstract out Platform specific entry point handling
#include "Error/Error.h"
#include "Application/Application.h"

extern Engine::CApplication* Engine::CreateApplication(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    Engine::CApplication* App = Engine::CreateApplication(argc, argv);
    Engine::SGenericError Error = App->Init();

    if (Error)
    {
        LOG(LogAny, Fatal, "Application Init failed: {}", Error);
        return Error.ErrorCode;
    }

    App->Run();
    App->Shutdown();

    delete App;
}
#endif