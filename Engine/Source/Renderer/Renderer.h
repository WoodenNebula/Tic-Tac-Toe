#pragma once

#include <GLFW/glfw3.h>

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include <filesystem>

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

    static void SetClearColor(const glm::vec4& Color);

    static void StartDraw();

    /// <summary>
    /// Binds the VA, assumes shader already bound and then draws the quads withusing indexCount
    /// </summary>
    /// <param name="VA"> Vertex Array Object</param>
    /// <param name="indexCount?"> if provided: Number of indices to draw from the VertexBuffer, 
    /// else uses the indexCount from VA.IBO
    /// </param>
    static void DrawIndexed(const std::shared_ptr<CVertexArray>& VA, uint32_t indexCount = 0);

    static void DrawLine(const glm::vec3& Start, const glm::vec3& End, const glm::vec4& Color, float Width);
    static void DrawQuad(const glm::mat4& Transform, const std::shared_ptr<CTexture>& Texture, const glm::vec4& TintColor = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawSprite(const glm::vec3& Position, const glm::vec2& Size, const std::filesystem::path& SpriteSource);
    static void Flush();
};
} // namespace Engine