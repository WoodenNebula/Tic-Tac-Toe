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

class CApplication
{
public:
    CApplication();
    CApplication(const SApplicationProps& inApplicationProps);
    ~CApplication();

    virtual SGenericError Init();

    virtual void OnEvent(Events::CEventBase& event);

    void PushLayer(CLayer* layer);
    void PushOverlay(CLayer* overlay);
    void PopLayer(CLayer* layer);
    void PopOverlay(CLayer* overlay);

    void Run();
    void Shutdown();

    void SubmitToMainThread(const std::function<void()>& func);

public:
    static inline CApplication* App{};
protected:
    virtual bool OnWindowCloseEvent(Events::CWindowCloseEvent& e);
    bool OnWindowResizeEvent(Events::WindowResizeEvent& e);
    bool OnWindowMovedEvent(Events::WindowMovedEvent& e);

    void ProcessPendingOperations();

    SApplicationProps m_ApplicationProps;
    bool m_IsRunning;

    std::unique_ptr<CWindow> m_Window;
    CLayerStack m_LayerStack;

    std::queue<std::function<void()>> m_PendingOperations;
};

// Entry point to be defined in GAME project
CApplication* CreateApplication(int argc, char* argv[]);
}; // !namespace Engine