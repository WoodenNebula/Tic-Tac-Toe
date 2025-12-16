#pragma once

#include "Error/Error.h"
#include "Window/Window.h"
#include <memory>

namespace Engine
{
struct SApplicationProps
{
    SWindowProps WindowProps;
    // Other Application specific props here
};

class Application
{
public:
    Application();
    Application(const SApplicationProps& inApplicationProps);
    ~Application();

    SGenericError Init();
    void Run();
    void Shutdown();
    void OnWindowQuitInput();

private:
    SApplicationProps m_ApplicationProps;
    std::unique_ptr<Window> m_Window;

    bool m_ShouldClose{ false };
};
}; // !namespace Engine