#pragma once

#include "Utils.h"
#include "Error/Error.h"
#include <functional>

struct GLFWwindow;

namespace Engine
{
    struct SWindowProps
    {
        Point2D<uint32_t> Dimension;
        Point2D<uint32_t> Position;
        std::string_view Title;
    };

    class Window
    {
    public:
        Window();
        Window(const SWindowProps& inWindowProps);
        ~Window();

        SGenericError Init();
        void Update();
        void Terminate();

        /// TODO: Move to Event System
        void RegisterWindowQuitInputCallback(const std::function<void()>& callback);

        void BroadCastWindowQuitInputCallback();

    private:
        SWindowProps m_WindowProps;
        GLFWwindow* m_WindowHandle;

        std::function<void()> m_WindowQuitInputCb;
    };
}