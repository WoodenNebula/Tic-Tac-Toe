#pragma once

#include "Core/Util/Utils.h"
#include "Error/Error.h"

#include "Events/Event.h"
#include "Logger/Logger.h"

#include <functional>

DECLARE_LOG_CATEGORY(Window)

struct GLFWwindow;

namespace Engine
{

using WindowEventCallbackFn = std::function<void(Events::EventBase&)>;

struct SWindowProps
{
    Point2D<uint32_t> Dimension;
    Point2D<int32_t> Position;
    std::string_view Title;

    WindowEventCallbackFn EventCallback;
};

class Window
{
public:

    Window();
    Window(const SWindowProps& inWindowProps);
    ~Window();

    SGenericError Init();
    void OnUpdate();
    void Terminate();

    void SetWindowEventCallback(const WindowEventCallbackFn& callback);
    void CloseWindow();
private:
    GLFWwindow* m_WindowHandle{ nullptr };
    SWindowProps m_WindowProps;
};


}