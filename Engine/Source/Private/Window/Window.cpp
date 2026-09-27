#include "Window/Window.h"

#include "Window/glfw_event_callbacks.h"

#include "Events/CoreEvents.h"

#include "Logger/Logger.h"

#include <string>
#include <glad/glad.h>

#include "Renderer/Renderer.h"
#include "GLFW/glfw3.h"

DECLARE_LOG_CATEGORY(GLFW);

namespace Engine
{

static void glfw_error_callback(int error_code, const char* description)
{
    LOG(LogGLFW, Error, "{}: {}", error_code, description);
}

CWindow::CWindow() {}

CWindow::CWindow(const SWindowProps& inWindowProps) : m_WindowProps(inWindowProps)
{
    /// TODO: Engine Init
    /// Engine::Core::Init()

    if (!glfwInit())
    {
        const char* errorDesc = "";
        int errorCode = glfwGetError(&errorDesc);
        LOG(LogGLFW, Fatal, "Initialization failed -> {}, {}", errorCode, errorDesc);
        abort();
    }

    glfwSetErrorCallback(glfw_error_callback);

    // set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /// !Engine::Core::Init()

    LOG(LogWindow, Trace, "Engine Core Init");
}

CWindow::~CWindow()
{
    Terminate();
    LOG(LogWindow, Trace, "Window Destroyed");
}

SGenericError CWindow::Init()
{
    LOG(LogWindow, Trace, "Window Init Called");

    m_WindowHandle = glfwCreateWindow(m_WindowProps.Dimension.x, m_WindowProps.Dimension.y, m_WindowProps.Title.data(), NULL, NULL);

    if (!m_WindowHandle)
    {
        const char* error_desc = "";
        int error_code = glfwGetError(&error_desc);

        return { error_code, error_desc };
    }
    LOG(LogWindow, Trace, "Window Created");

    // this creates a valid window context for opengl to render to
    CRenderer::InitGLContext(m_WindowHandle);

    glfwSetWindowUserPointer(m_WindowHandle, &m_WindowProps);

    // GLFW Callbacks
    glfwSetKeyCallback(m_WindowHandle, CGLFWEventCallbacks::key_callback);

    glfwSetMouseButtonCallback(m_WindowHandle, CGLFWEventCallbacks::mouse_button_callback);
    glfwSetScrollCallback(m_WindowHandle, CGLFWEventCallbacks::mouse_scroll_callback);
    glfwSetCursorPosCallback(m_WindowHandle, CGLFWEventCallbacks::mouse_move_callback);

    glfwSetWindowCloseCallback(m_WindowHandle, CGLFWEventCallbacks::window_close_callback);
    glfwSetWindowFocusCallback(m_WindowHandle, CGLFWEventCallbacks::window_focus_callback);
    glfwSetWindowSizeCallback(m_WindowHandle, CGLFWEventCallbacks::window_size_callback);
    glfwSetWindowPosCallback(m_WindowHandle, CGLFWEventCallbacks::window_pos_callback);


    if (!m_WindowProps.WindowIconPath.empty())
    {
        CImage windowIcon(m_WindowProps.WindowIconPath, true);
        if (!windowIcon.IsLoaded())
        {
            SGenericError err = windowIcon.Load();
            if (err) { return err; }
        }

        GLFWimage img{ .width = (int)windowIcon.Dimensions.x, .height = (int)windowIcon.Dimensions.y, .pixels = windowIcon.Data };
        glfwSetWindowIcon(m_WindowHandle, 1, &img);
    }
    else
    {
        LOG(LogWindow, Warning, "Missing Window Icon Path, proceeding with normal icon.");
    }
    return {};
}

void CWindow::OnUpdate(float dt)
{
    glfwPollEvents();
    glfwSwapBuffers(m_WindowHandle);
}

void CWindow::CloseWindow()
{
    if (m_WindowHandle)
    {
        glfwSetWindowShouldClose(m_WindowHandle, GLFW_TRUE);
        LOG(LogWindow, Trace, "Window Close Requested");
    }
}

void CWindow::Terminate()
{
    if (m_WindowHandle)
    {
        LOG(LogWindow, Trace, "Terminating Window");
        glfwDestroyWindow(m_WindowHandle);
        m_WindowHandle = nullptr;
    }
}

void CWindow::SetWindowTitle(const std::string& Title)
{
    if (m_WindowHandle)
    {
        m_WindowProps.Title = Title;
        glfwSetWindowTitle(m_WindowHandle, m_WindowProps.Title.data());
    }
}


void CWindow::SetWindowEventCallback(const WindowEventCallbackFn& callback)
{
    m_WindowProps.EventCallback = callback;
    LOG(LogWindow, Trace, "Window Event Callback Set");
}

}; // namespace Engine