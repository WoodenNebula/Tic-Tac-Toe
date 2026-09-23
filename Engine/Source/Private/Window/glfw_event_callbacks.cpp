#include "Window/glfw_event_callbacks.h"

#include "GLFW/glfw3.h"

#include "Window/Window.h"
#include "Events/CoreEvents.h"


namespace Engine
{
void CGLFWEventCallbacks::key_callback(GLFWwindow* windowHandle, int key, int scancode,
    int action, int mods)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    switch (action)
    {
    case GLFW_PRESS:
    {
        Events::InputEvents::CKeyPressedEvent pressEvent(static_cast<Events::EKey>(key), false);
        window.EventCallback(pressEvent);
        break;
    }
    case GLFW_RELEASE:
    {
        Events::InputEvents::CKeyReleasedEvent releaseEvent(static_cast<Events::EKey>(key));
        window.EventCallback(releaseEvent);
        break;
    }
    case GLFW_REPEAT:
    {
        Events::InputEvents::CKeyHeldEvent repeatEvent(static_cast<Events::EKey>(key));
        window.EventCallback(repeatEvent);
        break;
    }
    default:
        break;
    }
}

void CGLFWEventCallbacks::mouse_button_callback(GLFWwindow* windowHandle, int button, int action, int mods)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    switch (action)
    {
    case GLFW_PRESS:
    {
        Events::InputEvents::CMouseButtonPressedEvent pressEvent(static_cast<Events::EMouse>(button));
        window.EventCallback(pressEvent);
        break;
    }
    case GLFW_RELEASE:
    {
        Events::InputEvents::CMouseButtonReleasedEvent releaseEvent(static_cast<Events::EMouse>(button));
        window.EventCallback(releaseEvent);
        break;
    }
    default:
        break;
    }
}

void CGLFWEventCallbacks::mouse_scroll_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::InputEvents::CMouseScrolledEvent scrollEvent(xoffset, yoffset);
    window.EventCallback(scrollEvent);
}

void CGLFWEventCallbacks::mouse_move_callback(GLFWwindow* windowHandle, double xoffset, double yoffset)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::InputEvents::CMouseMovedEvent moveEvent(xoffset, yoffset);
    window.EventCallback(moveEvent);
}


void CGLFWEventCallbacks::window_pos_callback(GLFWwindow* windowHandle, int xpos, int ypos)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    window.Position = { (int32_t)xpos, (int32_t)ypos };

    Events::WindowMovedEvent movedEvent(xpos, ypos);
    window.EventCallback(movedEvent);
}

void CGLFWEventCallbacks::window_size_callback(GLFWwindow* windowHandle, int width, int height)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));
    window.Dimension = { (uint32_t)width, (uint32_t)height };

    Events::WindowResizeEvent resizeEvent(width, height);
    window.EventCallback(resizeEvent);
}

void CGLFWEventCallbacks::window_close_callback(GLFWwindow* windowHandle)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    Events::CWindowCloseEvent windowCloseEvent;
    window.EventCallback(windowCloseEvent);
}

void CGLFWEventCallbacks::window_focus_callback(GLFWwindow* windowHandle, int focused)
{
    SWindowProps& window = *static_cast<SWindowProps*>(glfwGetWindowUserPointer(windowHandle));

    if (focused == GLFW_TRUE)
    {
        Events::CWindowGainFocusEvent focusEvent;
        window.EventCallback(focusEvent);
    }
    else
    {
        Events::CWindowLostFocus focusEvent;
        window.EventCallback(focusEvent);
    }
}
};