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
Application::Application() : m_IsRunning(true) {}

Application::Application(const SApplicationProps& inApplicationProps) : m_ApplicationProps(inApplicationProps), m_IsRunning(true)
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
    // Reverse iterate through layer stack
    for (auto itr = m_LayerStack.end(); itr != m_LayerStack.begin(); )
    {
        auto layer = *(--itr);
        layer->OnEvent(event);
        if (event.Handled)
        {
            break;
        }
    }


    //switch (event.GetEventType())
    //{
    //case Events::EventTypes::WindowClose:
    //    OnWindowQuitInput();
    //    break;
    //default:
    //    Events::EventManager::Get().DispatchEvent(event);
    //    break;
    //}
}

void Application::OnWindowQuitInput()
{
    m_Window->CloseWindow();
    m_IsRunning = false;
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