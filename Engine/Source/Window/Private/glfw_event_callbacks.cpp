#include "Window/glfw_event_callbacks.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "Window/Window.h"
#include "Events/CoreEvents.h"


namespace Engine
{
void GLFWEventCallbacks::key_callback(GLFWwindow* windowHandle, int key, int scancode,
    int action, int mods)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    switch (action)
    {
    case GLFW_PRESS:
    {
        Events::InputEvents::KeyPressedEvent pressEvent(static_cast<Events::Key>(key), false);
        window.EventCallback(pressEvent);
        break;
    }
    case GLFW_RELEASE:
    {
        Events::InputEvents::KeyReleasedEvent releaseEvent(static_cast<Events::Key>(key));
        window.EventCallback(releaseEvent);
        break;
    }
    case GLFW_REPEAT:
    {
        Events::InputEvents::KeyHeldEvent repeatEvent(static_cast<Events::Key>(key));
        window.EventCallback(repeatEvent);
        break;
    }
    default:
        break;
    }
}

void GLFWEventCallbacks::mouse_button_callback(GLFWwindow* windowHandle, int button, int action, int mods)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    switch (action)
    {
    case GLFW_PRESS:
    {
        Events::InputEvents::MouseButtonPressedEvent pressEvent(static_cast<Events::Mouse>(button));
        window.EventCallback(pressEvent);
        break;
    }
    case GLFW_RELEASE:
    {
        Events::InputEvents::MouseButtonReleasedEvent releaseEvent(static_cast<Events::Mouse>(button));
        window.EventCallback(releaseEvent);
        break;
    }
    default:
        break;
    }
}

void GLFWEventCallbacks::mouse_scroll_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::InputEvents::MouseScrolledEvent scrollEvent(xoffset, yoffset);
    window.EventCallback(scrollEvent);
}

void GLFWEventCallbacks::mouse_move_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::InputEvents::MouseMovedEvent moveEvent(xoffset, yoffset);
    window.EventCallback(moveEvent);
}


void GLFWEventCallbacks::window_pos_callback(GLFWwindow* windowHandle, int xpos, int ypos)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    window.Position = { (int32_t)xpos, (int32_t)ypos };

    Events::WindowMovedEvent movedEvent(xpos, ypos);
    window.EventCallback(movedEvent);
}

void GLFWEventCallbacks::window_size_callback(GLFWwindow* windowHandle, int width, int height)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    window.Dimension = { (uint32_t)width, (uint32_t)height };

    Events::WindowResizeEvent resizeEvent(width, height);
    window.EventCallback(resizeEvent);
}

void GLFWEventCallbacks::window_close_callback(GLFWwindow* windowHandle)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::WindowCloseEvent windowCloseEvent;
    window.EventCallback(windowCloseEvent);
}

void GLFWEventCallbacks::window_focus_callback(GLFWwindow* windowHandle, int focused)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    if (focused == GLFW_TRUE)
    {
        Events::WindowGainFocusEvent focusEvent;
        window.EventCallback(focusEvent);
    }
    else
    {
        Events::WindowLostFocus focusEvent;
        window.EventCallback(focusEvent);
    }
}
};