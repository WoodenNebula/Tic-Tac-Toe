#include "Window/Window.h"

#include "Events/EventManager.h"
#include "Events/CoreEvents.h"

#include "Logger/Logger.h"

#include <string>
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#ifdef DEBUG
#include "Core/Util/magic_enum.hpp"
#endif

namespace Engine
{

static void glfw_error_callback(int error_code, const char* description)
{
    std::string errorMsg = std::string(description) + ", " + std::to_string(error_code);
    Logger::LogError("GLFW", errorMsg);
}

static void key_callback(GLFWwindow* windowHandle, int key, int scancode,
    int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        switch (action)
        {
        case GLFW_PRESS:
        {
            Events::InputEvents::KeyPressedEvent pressEvent(static_cast<Events::Key>(key), false);
            window->HandleGLFWEvents(pressEvent);
            break;
        }
        case GLFW_RELEASE:
        {
            Events::InputEvents::KeyReleasedEvent releaseEvent(static_cast<Events::Key>(key));
            window->HandleGLFWEvents(releaseEvent);
            break;
        }
        default:
            break;
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
        if (window)
        {
            window->BroadCastWindowQuitInputCallback();
        }
    }
}

static void mouse_button_callback(GLFWwindow* windowHandle, int button, int action, int mods)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        switch (action)
        {
        case GLFW_PRESS:
        {
            Events::InputEvents::MouseButtonPressedEvent pressEvent(static_cast<Events::Mouse>(button));
            window->HandleGLFWEvents(pressEvent);
            break;
        }
        case GLFW_RELEASE:
        {
            Events::InputEvents::MouseButtonReleasedEvent releaseEvent(static_cast<Events::Mouse>(button));
            window->HandleGLFWEvents(releaseEvent);
            break;
        }
        default:
            break;
        }
    }
}

static void mouse_scroll_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::InputEvents::MouseScrolledEvent scrollEvent(xoffset, yoffset);
        window->HandleGLFWEvents(scrollEvent);
    }
}

static void mouse_move_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::InputEvents::MouseMovedEvent moveEvent(xoffset, yoffset);
        window->HandleGLFWEvents(moveEvent);
    }
}


Window::Window() {}

Window::Window(const SWindowProps& inWindowProps) : m_WindowProps(inWindowProps)
{
    /// TODO: Engine Init
    /// Engine::Core::Init()
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        const char* errorDesc = "";
        int errorCode = glfwGetError(&errorDesc);
        std::string errorMsg = "GLFW Initialization failed: " + std::string(errorDesc) + ", " + std::to_string(errorCode);
        Logger::LogError("Window-GLFW", errorMsg);
        abort();
    }

    // set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /// !Engine::Core::Init()

    Logger::LogInfo("Window-GLFW", "Engine Core Init");
}

Window::~Window()
{
    Terminate();
    Logger::LogInfo("Window", "Window Destroyed");
}

SGenericError Window::Init()
{
    Logger::LogInfo("Window-GLFW", "Window Init Called");

    m_WindowHandle = glfwCreateWindow(m_WindowProps.Dimension.x, m_WindowProps.Dimension.y, m_WindowProps.Title.data(), NULL, NULL);

    if (!m_WindowHandle)
    {
        const char* error_desc = "";
        int error_code = glfwGetError(&error_desc);

        return { error_code, error_desc };
    }
    Logger::LogInfo("Window-GLFW", "Window Created");

    glfwSetWindowUserPointer(m_WindowHandle, this);

    glfwSetKeyCallback(m_WindowHandle, key_callback);

    glfwSetMouseButtonCallback(m_WindowHandle, mouse_button_callback);
    glfwSetScrollCallback(m_WindowHandle, mouse_scroll_callback);
    glfwSetCursorPosCallback(m_WindowHandle, mouse_move_callback);

    // this creates a valid window context for opengl to render to
    glfwMakeContextCurrent(m_WindowHandle);

    // glad should only be initialized after a valid context has been created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return { -1, "GLAD LoadGlLoader Failed" };
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
    if (m_WindowHandle)
    {
        Logger::LogInfo("Window-GLFW", "Window Terminated");
        glfwDestroyWindow(m_WindowHandle);
        m_WindowHandle = nullptr;
    }
}

void OnEvent(const Events::EventBase& event)
{
    //switch (event.GetEventCategory())
    //{
    //    case Events::A

    //}
}

void Window::BroadCastWindowQuitInputCallback()
{
    if (m_WindowQuitInputCb)
        m_WindowQuitInputCb();
}

void Window::RegisterWindowQuitInputCallback(const std::function<void()>& callback)
{
    if (callback)
    {
        m_WindowQuitInputCb = callback;
    }
}

void Window::HandleGLFWEvents(const Events::EventBase& event)
{
    Events::EventManager::Get().DispatchEvent(event);
}

}; // namespace Engine