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

    virtual void OnEvent(Events::EventBase& event);

    void Run();
    void Shutdown();

public:
    static inline Application* App{};
protected:
    virtual bool OnWindowCloseEvent(Events::WindowCloseEvent& e);
    bool OnWindowResizeEvent(Events::WindowResizeEvent& e);
    bool OnWindowMovedEvent(Events::WindowMovedEvent& e);

    SApplicationProps m_ApplicationProps;
    bool m_IsRunning;

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;

};

// Entry point to be defined in GAME project
Application* CreateApplication(int argc, char* argv[]);
}; // !namespace Engine