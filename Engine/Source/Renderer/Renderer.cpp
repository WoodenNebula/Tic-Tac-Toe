#include "Renderer.h"

#include "Logger/Logger.h"
#include <iostream>

namespace Engine
{

DECLARE_LOG_CATEGORY(Renderer)

void OpenGLMessageCallback(
    unsigned source,
    unsigned type,
    unsigned id,
    unsigned severity,
    int length,
    const char* message,
    const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         LOG(Renderer, ERROR, "{}", message); return;
    case GL_DEBUG_SEVERITY_MEDIUM:       LOG(Renderer, WARN, "{}", message); return;
    case GL_DEBUG_SEVERITY_LOW:          LOG(Renderer, INFO, "{}", message); return;
    case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(Renderer, TRACE, "{}", message); return;
    default:                             LOG(Renderer, ERROR, "Unknown severity level!"); return;
    }

    //ASSERT(false, "Unknown severity level!");
}


void Renderer::Init()
{
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    LOG(Renderer, TRACE, "Renderer Initialized");
}

void Renderer::InitGLContext(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    LOG(Renderer, TRACE, "GLFW CONTEXT INIT START");

    // glad should only be initialized after a valid context has been created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(Renderer, FATAL, "GLAD LoadGlLoader Failed");
    }
    LOG(Renderer, TRACE, "GLFW CONTEXT Initialized");
}


void Renderer::LineMode(bool drawInLineMode = false)
{
    if (drawInLineMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


void Renderer::Clear()
{
/* Clean and assign new color to back buffer*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(const glm::vec3& Color)
{
    glClearColor(Color.r, Color.g, Color.b, 1.0f);
}

void Renderer::Draw(const VertexArray& VA, const IndexBuffer& IBO,
    const Shader& shader)
{
/* Color of background of window */
    glClearColor(0.2f, 0.39f, 0.32f, 1.0f);

    shader.Bind();
    VA.Bind();
    IBO.Bind();

    ///
    /// Primitive(shape) we want to use
    /// How many indices we want to draw
    /// Data type of indices
    /// Index of indices
    ///
    glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, 0);
}

static void DrawLine(const VertexArray& VA, uint32_t vertexCount)
{
    VA.Bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}


} // namespace Engine