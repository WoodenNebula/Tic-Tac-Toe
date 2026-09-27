#include "Application/Application.h"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>

#include "Events/CoreEvents.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"

DECLARE_LOG_CATEGORY(Application)

namespace Engine
{

CApplication::CApplication() : m_IsRunning(true) {}

CApplication::CApplication(const SApplicationProps& inApplicationProps) : m_ApplicationProps(inApplicationProps), m_IsRunning(true)
{
    // Application specific pre init code here
}

CApplication::~CApplication()
{
    // TODO: DEINIT APPLICATION AND THEN CORE
    if (m_IsRunning)
    {
        Shutdown();
    }
    LOG(LogApplication, Trace, "Application Shutdown Complete");
}

SGenericError CApplication::Init()
{
    m_Window = std::make_unique<CWindow>(m_ApplicationProps.WindowProps);
    SGenericError Err = m_Window->Init();

    if (Err)
    {
        return Err;
    }
    LOG(LogWindow, Success, "Window Init");

    m_Window->SetWindowEventCallback(BIND_EVENT_CB(CApplication::OnEvent));
    LOG(LogApplication, Trace, "Event callback bound to window events");

    CRenderer::Init();
    return {};
}


void CApplication::OnEvent(Events::CEventBase& event)
{
    Events::CEventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Events::CWindowCloseEvent>(BIND_EVENT_CB(CApplication::OnWindowCloseEvent));
    dispatcher.DispatchEvent<Events::CWindowMovedEvent>(BIND_EVENT_CB(CApplication::OnWindowMovedEvent));
    dispatcher.DispatchEvent<Events::CWindowResizeEvent>(BIND_EVENT_CB(CApplication::OnWindowResizeEvent));


    // Reverse iterate through layer stack
    for (auto itr = m_LayerStack.end(); itr != m_LayerStack.begin(); )
    {
        if (event.Handled)
        {
            break;
        }
        auto layer = *(--itr);
        layer->OnEvent(event);
    }

    // Process any pending operations after event handling is complete
    ProcessPendingOperations();
}

void CApplication::PushLayer(CLayer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}
void CApplication::PushOverlay(CLayer* overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void CApplication::PopLayer(CLayer* layer)
{
    m_LayerStack.PopLayer(layer);
    layer->OnDetach();
}

void CApplication::PopOverlay(CLayer* overlay)
{
    m_LayerStack.PopOverlay(overlay);
    overlay->OnDetach();
}

void CApplication::UpdateApplicationTitle(const std::string& newTitle)
{
    this->m_ApplicationProps.WindowProps.Title = newTitle;
    auto deferedFunc = [&]() {    this->m_Window->SetWindowTitle(this->m_ApplicationProps.WindowProps.Title); };
    SubmitToMainThread(deferedFunc);
}

bool CApplication::OnWindowCloseEvent(Events::CWindowCloseEvent& e)
{
    e.Handled = true;
    m_IsRunning = false;

    return true;
}

bool CApplication::OnWindowResizeEvent(Events::CWindowResizeEvent& e)
{
    e.Handled = true;
    m_ApplicationProps.WindowProps.Dimension = e.GetDimensions();

    CRenderer::SetViewport(0, 0, e.GetDimensions().x, e.GetDimensions().y);
    return true;
}

bool CApplication::OnWindowMovedEvent(Events::CWindowMovedEvent& e)
{
    e.Handled = true;
    m_ApplicationProps.WindowProps.Position = e.GetPosition();

    return true;
}


void CApplication::SubmitToMainThread(const std::function<void()>& func)
{
    m_PendingOperations.push(func);
}

void CApplication::ProcessPendingOperations()
{
    while (!m_PendingOperations.empty())
    {
        auto& operation = m_PendingOperations.front();
        operation();
        m_PendingOperations.pop();
    }
}

void CApplication::Update()
{
    for (auto layer : m_LayerStack)
    {
        layer->OnUpdate(0.0f);
    }
    m_Window->OnUpdate(0.0f);
}

void CApplication::Run()
{
    /// Engine Loop first?
    while (m_IsRunning)
    {
        Update();
        // Process any pending operations at the end of each frame
        ProcessPendingOperations();
    }
}

void CApplication::Shutdown()
{
    m_Window->Terminate();

    /// TODO: Engine Shutdown thing
    //std::cout << "Application Shutdown\n";

    //glfwTerminate();
    //std::cout << "GLFW Terminated\n";

}
};