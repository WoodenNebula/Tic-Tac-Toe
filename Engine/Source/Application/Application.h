#pragma once

#include "Error/Error.h"
#include "Window/Window.h"
#include "Core/LayerStack.h"
#include "Events/CoreEvents.h"

#include <memory>

namespace Engine
{
struct SApplicationProps
{
    SWindowProps WindowProps;
    // Other Application specific props here
};

class Application
{
public:
    Application();
    Application(const SApplicationProps& inApplicationProps);
    ~Application();

    SGenericError Init();

    void OnEvent(Events::EventBase& event);

    void Run();
    void Shutdown();

protected:
    bool OnWindowCloseEvent(Events::WindowCloseEvent& e);

    SApplicationProps m_ApplicationProps;
    bool m_IsRunning;

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
};

// Entry point to be defined in GAME project
Application* CreateApplication(int argc, char* argv[]);
}; // !namespace Engine