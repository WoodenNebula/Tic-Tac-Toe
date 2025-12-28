#include "Application/Application.h"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>

#include "Events/EventManager.h"
#include "Events/CoreEvents.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"

DECLARE_LOG_CATEGORY(Application)

namespace Engine
{
Application::Application() : m_IsRunning(true) {}

Application::Application(const SApplicationProps& inApplicationProps) : m_ApplicationProps(inApplicationProps), m_IsRunning(true)
{
    // Application specific pre init code here
}

Application::~Application()
{
    // TODO: DEINIT APPLICATION AND THEN CORE
    if (m_IsRunning)
    {
        Shutdown();
    }
    LOG(Application, TRACE, "Application Shutdown Complete");
}

SGenericError Application::Init()
{
    m_Window = std::make_unique<Window>(m_ApplicationProps.WindowProps);
    SGenericError Err = m_Window->Init();
    std::cout << "Window Init\n";

    if (Err)
    {
        return Err;
    }

    m_Window->SetWindowEventCallback(BIND_EVENT_CB(Application::OnEvent));
    LOG(Application, TRACE, "Event callback bound to window events");

    Renderer::Init();
    return {};
}


void Application::OnEvent(Events::EventBase& event)
{
    Events::EventDispatcher dispatcher(event);
    dispatcher.DispatchEvent<Events::WindowCloseEvent>(BIND_EVENT_CB(Application::OnWindowCloseEvent));
    dispatcher.DispatchEvent<Events::WindowMovedEvent>(BIND_EVENT_CB(Application::OnWindowMovedEvent));
    dispatcher.DispatchEvent<Events::WindowResizeEvent>(BIND_EVENT_CB(Application::OnWindowResizeEvent));


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
}

bool Application::OnWindowCloseEvent(Events::WindowCloseEvent& e)
{
    e.Handled = true;
    m_IsRunning = false;

    return true;
}

bool Application::OnWindowResizeEvent(Events::WindowResizeEvent& e)
{
    e.Handled = true;
    m_ApplicationProps.WindowProps.Dimension = e.GetDimensions();

    return true;
}

bool Application::OnWindowMovedEvent(Events::WindowMovedEvent& e)
{
    e.Handled = true;
    m_ApplicationProps.WindowProps.Position = e.GetPosition();

    return true;
}


void Application::Run()
{
    /// Engine Loop first?
    while (m_IsRunning)
    {
        for (auto layer : m_LayerStack)
        {
            layer->OnUpdate(0.0f);
        }
        m_Window->OnUpdate(0.0f);
        glfwPollEvents();
    }
}

void Application::Shutdown()
{
    m_Window->Terminate();

    /// TODO: Engine Shutdown thing
    //std::cout << "Application Shutdown\n";

    //glfwTerminate();
    //std::cout << "GLFW Terminated\n";

}
};