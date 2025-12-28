#pragma once

#include <memory>

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "glm.hpp"

namespace Engine
{

class Quad2D
{
private:
    glm::vec2 m_Center;
    float m_width, m_height;

    float m_vertices[16];
    uint32_t m_indices[6];

    std::vector<glm::vec3> m_initialPosition;

    std::unique_ptr<Shader> m_QuadShader;
    std::unique_ptr<VertexArray> m_VA;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<IndexBuffer> m_IB;
    std::unique_ptr<VertexBufferLayout> m_Layout;
    std::unique_ptr<Texture> m_Texture;
    glm::vec2 m_WinDimensions;

public:
 /// <summary>
 /// Generates a Quad in 2D space at the specified position with diameter
 /// scaled by given scalar
 /// </summary>
 /// <param name="center">Position at screen space(pixel) at which the quad
 /// will have its center</param> <param name="scale"></param>
    Quad2D(const glm::vec2& center, float width, float height,
        const std::string& texturePath);

    ~Quad2D();

    void Draw() const;

    bool ClickCollision(float cursorX, float cursorY) const;

    void BindShader() const;

    std::vector<glm::vec3> GetInitialPosition() const;

    // Uniform Setters
    void SetUniform1f(const std::string& name, float value) const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2,
        float v3) const;
    void SetUniformMat4f(const std::string& name,
        const glm::mat4& matrix) const;
};
} // namespace Engine