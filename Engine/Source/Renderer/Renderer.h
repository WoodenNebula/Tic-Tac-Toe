#pragma once

#include <GLFW/glfw3.h>

#include "Buffer.h"
#include "Shader.h"
#include "VertexArray.h"

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

    /// <summary>
    /// Binds the VA, IBO, uses shaderprogram and finally renders the vertices
    /// as triangles
    /// </summary>
    /// <param name="VA">: Vertex Array Object</param>
    /// <param name="IBO">: Elemnt/Index Buffer Object</param>
    /// <param name="shader">: Shader Object</param>
    static void Draw(const VertexArray& VA, const IndexBuffer& IBO,
        const Shader& shader);

    static void DrawLine(const VertexArray& VA, uint32_t vertexCount);
};
} // namespace Engine