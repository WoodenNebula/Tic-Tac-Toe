#include "Application/Application.h"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <iostream>

namespace Engine
{
    Application::Application() {}

    Application::Application(const SApplicationProps &inApplicationProps) : m_ApplicationProps(inApplicationProps)
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

        m_Window->RegisterWindowQuitInputCallback(std::bind(&Application::OnWindowQuitInput, this));

        std::cout << "Window Quit Cb Bound\n";

        return {};
    }

    void Application::OnWindowQuitInput()
    {
        m_ShouldClose = true;
    }

    void Application::Run()
    {
        /// Engine Loop first?
        while (!m_ShouldClose)
        {
            m_Window->Update();
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