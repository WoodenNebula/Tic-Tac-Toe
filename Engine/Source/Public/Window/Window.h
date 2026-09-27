#pragma once

#include "AssetManager/Image.h"

#include "Core/Util/Utils.h"
#include "Error/Error.h"

#include "Events/Event.h"
#include "Logger/Logger.h"

#include <functional>

DECLARE_LOG_CATEGORY(Window)

struct GLFWwindow;

namespace Engine
{

using WindowEventCallbackFn = std::function<void(Events::CEventBase&)>;

struct SWindowProps
{
    Point2D<uint32_t> Dimension;
    Point2D<int32_t> Position;
    std::string Title;

    FPath WindowIconPath;

    WindowEventCallbackFn EventCallback;
};

class CWindow
{
public:

    CWindow();
    CWindow(const SWindowProps& inWindowProps);
    ~CWindow();

    SGenericError Init();
    void OnUpdate(float dt);
    void Terminate();

    void SetWindowTitle(const std::string& Title);

    void SetWindowEventCallback(const WindowEventCallbackFn& callback);
    void CloseWindow();

    GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }
private:
    GLFWwindow* m_WindowHandle{ nullptr };
    SWindowProps m_WindowProps;
};


}