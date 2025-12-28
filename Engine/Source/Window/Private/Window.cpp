#include "Window/Window.h"

#include "Window/glfw_event_callbacks.h"

#include "Events/EventManager.h"
#include "Events/CoreEvents.h"

#include "Logger/Logger.h"

#include <string>
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

DECLARE_LOG_CATEGORY(GLFW);

namespace Engine
{

static void glfw_error_callback(int error_code, const char* description)
{
    LOG(GLFW, ERROR, "{}: {}", error_code, description);
}

Window::Window() {}

Window::Window(const SWindowProps& inWindowProps) : m_WindowProps(inWindowProps)
{
    /// TODO: Engine Init
    /// Engine::Core::Init()

    if (!glfwInit())
    {
        const char* errorDesc = "";
        int errorCode = glfwGetError(&errorDesc);
        LOG(GLFW, FATAL, "Initialization failed -> {}, {}", errorCode, errorDesc);
        abort();
    }

    glfwSetErrorCallback(glfw_error_callback);

    // set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /// !Engine::Core::Init()

    LOG(Window, TRACE, "Engine Core Init");
}

Window::~Window()
{
    Terminate();
    LOG(Window, TRACE, "Window Destroyed");
}

SGenericError Window::Init()
{
    LOG(Window, TRACE, "Window Init Called");

    m_WindowHandle = glfwCreateWindow(m_WindowProps.Dimension.x, m_WindowProps.Dimension.y, m_WindowProps.Title.data(), NULL, NULL);

    if (!m_WindowHandle)
    {
        const char* error_desc = "";
        int error_code = glfwGetError(&error_desc);

        return { error_code, error_desc };
    }
    LOG(Window, TRACE, "Window Created");

    glfwSetWindowUserPointer(m_WindowHandle, &m_WindowProps);

    // GLFW Callbacks
    glfwSetKeyCallback(m_WindowHandle, GLFWEventCallbacks::key_callback);

    glfwSetMouseButtonCallback(m_WindowHandle, GLFWEventCallbacks::mouse_button_callback);
    glfwSetScrollCallback(m_WindowHandle, GLFWEventCallbacks::mouse_scroll_callback);
    glfwSetCursorPosCallback(m_WindowHandle, GLFWEventCallbacks::mouse_move_callback);

    glfwSetWindowCloseCallback(m_WindowHandle, GLFWEventCallbacks::window_close_callback);
    glfwSetWindowFocusCallback(m_WindowHandle, GLFWEventCallbacks::window_focus_callback);
    glfwSetWindowSizeCallback(m_WindowHandle, GLFWEventCallbacks::window_size_callback);
    glfwSetWindowPosCallback(m_WindowHandle, GLFWEventCallbacks::window_pos_callback);

    // this creates a valid window context for opengl to render to
    glfwMakeContextCurrent(m_WindowHandle);

    // glad should only be initialized after a valid context has been created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return { -1, "GLAD LoadGlLoader Failed" };
    }

    return {};
}

void Window::OnUpdate()
{
    glClearColor(.69f, .69f, .69f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_WindowHandle);
}

void Window::CloseWindow()
{
    if (m_WindowHandle)
    {
        glfwSetWindowShouldClose(m_WindowHandle, GLFW_TRUE);
        LOG(Window, TRACE, "Window Close Requested");
    }
}

void Window::Terminate()
{
    if (m_WindowHandle)
    {
        LOG(Window, TRACE, "Terminating Window");
        glfwDestroyWindow(m_WindowHandle);
        m_WindowHandle = nullptr;
    }
}


void Window::SetWindowEventCallback(const WindowEventCallbackFn& callback)
{
    m_WindowProps.EventCallback = callback;
    LOG(Window, TRACE, "Window Event Callback Set");
}

}; // namespace Engine