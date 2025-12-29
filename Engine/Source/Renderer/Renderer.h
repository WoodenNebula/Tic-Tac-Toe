#pragma once

#include <GLFW/glfw3.h>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

#include "glm.hpp"

namespace Engine
{

class Renderer
{
public:

    static void Init();
    static void InitGLContext(GLFWwindow* window);

    // Hint OpenGl to draw only the borders
    static void LineMode(bool drawInLineMode);

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    static void Clear();

    static void SetClearColor(const glm::vec3& Color);

    static void StartDraw();

    /// <summary>
    /// Binds the VA, IBO, uses shaderprogram and finally renders the vertices
    /// as triangles
    /// </summary>
    /// <param name="VA">: Vertex Array Object</param>
    /// <param name="IBO">: Elemnt/Index Buffer Object</param>
    /// <param name="shader">: Shader Object</param>
    static void Draw(const CVertexArray& VA, const CIndexBuffer& IBO,
        const CShader& shader);

    static void DrawLine(const glm::vec3& Start, const glm::vec3& End, const glm::vec4& Color, float Width);
    static void Flush();
};
} // namespace Engine