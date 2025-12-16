#include "Window/glfw_event_callbacks.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "Window/Window.h"
#include "Events/CoreEvents.h"


namespace Engine::GLFWEventCallbacks
{
void key_callback(GLFWwindow* windowHandle, int key, int scancode,
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

void mouse_button_callback(GLFWwindow* windowHandle, int button, int action, int mods)
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

void mouse_scroll_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::InputEvents::MouseScrolledEvent scrollEvent(xoffset, yoffset);
        window->HandleGLFWEvents(scrollEvent);
    }
}

void mouse_move_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::InputEvents::MouseMovedEvent moveEvent(xoffset, yoffset);
        window->HandleGLFWEvents(moveEvent);
    }
}


void window_pos_callback(GLFWwindow* windowHandle, int xpos, int ypos)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::WindowMovedEvent movedEvent(xpos, ypos);
        window->HandleGLFWEvents(movedEvent);
    }
}

void window_size_callback(GLFWwindow* windowHandle, int width, int height)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::WindowResizeEvent resizeEvent(width, height);
        window->HandleGLFWEvents(resizeEvent);
    }
}

void Engine::GLFWEventCallbacks::window_close_callback(GLFWwindow* windowHandle)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        Events::WindowCloseEvent windowCloseEvent;
        window->HandleGLFWEvents(windowCloseEvent);
    }
}

void window_focus_callback(GLFWwindow* windowHandle, int focused)
{
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(windowHandle));
    if (window)
    {
        if (focused == GLFW_TRUE)
        {
            Events::WindowGainFocusEvent focusEvent;
            window->HandleGLFWEvents(focusEvent);
        }
        else
        {
            Events::WindowLostFocus focusEvent;
            window->HandleGLFWEvents(focusEvent);
        }
    }
}
};