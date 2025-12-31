#pragma once

#include "Error/Error.h"
#include "Window/Window.h"
#include "Core/LayerStack.h"
#include "Events/CoreEvents.h"

#include <memory>
#include <functional>
#include <queue>

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

    virtual SGenericError Init();

    virtual void OnEvent(Events::EventBase& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    void Run();
    void Shutdown();

    void SubmitToMainThread(const std::function<void()>& func);

public:
    static inline Application* App{};
protected:
    virtual bool OnWindowCloseEvent(Events::WindowCloseEvent& e);
    bool OnWindowResizeEvent(Events::WindowResizeEvent& e);
    bool OnWindowMovedEvent(Events::WindowMovedEvent& e);

    void ProcessPendingOperations();

    SApplicationProps m_ApplicationProps;
    bool m_IsRunning;

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;

    std::queue<std::function<void()>> m_PendingOperations;
};

// Entry point to be defined in GAME project
Application* CreateApplication(int argc, char* argv[]);
}; // !namespace Engine