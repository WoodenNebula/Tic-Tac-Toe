#pragma once

#include "Error/Error.h"
#include "Window/Window.h"
#include "Core/LayerStack.h"
#include "Events/CoreEvents.h"

#include "Core/SubsystemBase.h"

#include <memory>
#include <functional>
#include <queue>


namespace Engine
{
struct SApplicationProps
{
    SWindowProps WindowProps;
    std::string Username;
    // Other Application specific props here
};

class CApplication : public ISubsystem
{
public:
    CApplication();
    CApplication(const SApplicationProps& inApplicationProps);
    virtual ~CApplication();

    //////////////////////////////////
    ///* Subsystem base interface *///
    //////////////////////////////////
    virtual SGenericError Init() override;
    virtual void Update() override;
    virtual void Shutdown() override;

    virtual void OnEvent(Events::CEventBase& event) override;

    virtual void Run();

    void PushLayer(CLayer* layer);
    void PushOverlay(CLayer* overlay);
    void PopLayer(CLayer* layer);
    void PopOverlay(CLayer* overlay);

    void UpdateApplicationTitle(const std::string& newTitle);

    inline Point2D<uint32_t> GetWindowDimensions() const
    {
        return m_ApplicationProps.WindowProps.Dimension;
    }

    void SubmitToMainThread(const std::function<void()>& func);

public:
    static inline CApplication* App{};
protected:
    virtual bool OnWindowCloseEvent(Events::CWindowCloseEvent& e);
    bool OnWindowResizeEvent(Events::CWindowResizeEvent& e);
    bool OnWindowMovedEvent(Events::CWindowMovedEvent& e);

    void ProcessPendingOperations();

protected:
    SApplicationProps m_ApplicationProps;
    bool m_IsRunning;

    std::unique_ptr<CWindow> m_Window;
    CLayerStack m_LayerStack;

    std::queue<std::function<void()>> m_PendingOperations;
};

// Entry point to be defined in GAME project
CApplication* CreateApplication(int argc, char* argv[]);
}; // !namespace Engine