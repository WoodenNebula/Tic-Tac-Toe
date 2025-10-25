#include "Window/Window.h"
#include <string>
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>

namespace Engine
{
    static void key_callback(GLFWwindow *windowHandle, int key, int scancode,
                             int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
            Window *window = static_cast<Window *>(glfwGetWindowUserPointer(windowHandle));
            if (window)
            {
                window->BroadCastWindowQuitInputCallback();
            }
        }
    }

    Window::Window() {}

    Window::Window(const SWindowProps &inWindowProps) : m_WindowProps(inWindowProps)
    {
        /// TODO: Engine Init
        /// Engine::Core::Init()
        if (!glfwInit())
        {
            const char* error_desc = "";
            int error_code = glfwGetError(&error_desc);
            std::cerr << "[ERROR-GLFW] Initialization failed" << error_desc << ", "
                << error_code << std::endl;
            abort();
        }

        // set opengl version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        /// !Engine::Core::Init()

        std::cout << "Engine Core Init\n";
    }

    Window::~Window()
    {
        Terminate();
        std::cout << "Window Destroyed\n";
    }

    SGenericError Window::Init()
    {
        std::cout << "Window Init Called\n";

        m_WindowHandle = glfwCreateWindow(m_WindowProps.Dimension.x, m_WindowProps.Dimension.y, m_WindowProps.Title.data(), NULL, NULL);

        std::cout << "Window Created\n";


        if (!m_WindowHandle)
        {
            const char *error_desc = "";
            int error_code = glfwGetError(&error_desc);

            return {error_code, error_desc};
        }

        glfwSetWindowUserPointer(m_WindowHandle, this);
        glfwSetKeyCallback(m_WindowHandle, key_callback);

        // this creates a valid window context for opengl to render to
        glfwMakeContextCurrent(m_WindowHandle);

        // glad should only be initialized after a valid context has been created
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            return {-1, "GLAD LoadGlLoader Failed"};
        }

        return {};
    }

    void Window::Update()
    {
        glClearColor(.69, .69, .69, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_WindowHandle);
    }

    void Window::Terminate()
    {
        std::cout << "Window Terminated\n";
        glfwDestroyWindow(m_WindowHandle);
    }

    void Window::BroadCastWindowQuitInputCallback()
    {
        if (m_WindowQuitInputCb)
            m_WindowQuitInputCb();
    }

    void Window::RegisterWindowQuitInputCallback(const std::function<void()> &callback)
    {
        if (callback)
        {
            m_WindowQuitInputCb = callback;
        }
    }

}