#include "Application/Application.h"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>

#include "Events/EventManager.h"
#include "Events/CoreEvents.h"
#include "Logger/Logger.h"

DECLARE_LOG_CATEGORY(Application)

namespace Engine
{
Application::Application() {}

Application::Application(const SApplicationProps& inApplicationProps) : m_ApplicationProps(inApplicationProps)
{
    // Application specific pre init code here
}

Application::~Application()
{
    // TODO: DEINIT APPLICATION AND THEN CORE
    Shutdown();
    std::cout << "Application Destroyed\n";

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

    m_Window->SetWindowEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    LOG(Application, TRACE, "Event callback bound to window events");

    return {};
}


void Application::OnEvent(const Events::EventBase& event)
{
    switch (event.GetEventType())
    {
    case Events::EventTypes::WindowClose:
        OnWindowQuitInput();
        break;
    default:
        Events::EventManager::Get().DispatchEvent(event);
        break;
    }
}

void Application::OnWindowQuitInput()
{
    m_Window->CloseWindow();
    m_ShouldClose = true;
}

void Application::Run()
{
    /// Engine Loop first?
    while (!m_ShouldClose)
    {
        m_Window->OnUpdate();
        glfwPollEvents();
    }
}

void Application::Shutdown()
{
    m_Window->Terminate();

    /// TODO: Engine Shutdown thing
    std::cout << "Application Shutdown\n";

    glfwTerminate();
    std::cout << "GLFW Terminated\n";

}
};