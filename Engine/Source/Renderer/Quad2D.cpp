#include "Quad2D.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

namespace Engine
{

Quad2D::Quad2D(const glm::vec2& center, float width, float height,
    const std::string& texturePath)
    : m_Center(center),
    m_width(width),
    m_height(height),
    // clang-format off
    m_vertices{
        // Positions                                 // Texture coord (Bottom left is 0,0)
        m_Center.x - m_width, m_Center.y + m_height, 0.0f, 1.0f,  // 0 Top Left
        m_Center.x + m_width, m_Center.y + m_height, 1.0f, 1.0f,  // 1 Top Right
        m_Center.x + m_width, m_Center.y - m_height, 1.0f, 0.0f,  // 2 Bottom Right
        m_Center.x - m_width, m_Center.y - m_height, 0.0f, 0.0f  // 3 Bottom Left
    },
    m_initialPosition{
        glm::vec3(m_vertices[0], m_vertices[1], 0.0f),   // 0
        glm::vec3(m_vertices[4], m_vertices[5], 0.0f),   // 1
        glm::vec3(m_vertices[8], m_vertices[9], 0.0f),   // 2
        glm::vec3(m_vertices[12], m_vertices[13], 0.0f)  // 3
    },
    m_indices{ 0, 1, 2, 2, 3, 0 }
{  // clang-format on
    m_VA.reset(new VertexArray(sizeof(m_vertices)));
    m_VB.reset(new VertexBuffer(m_vertices, sizeof(m_vertices)));

    m_IB.reset(new IndexBuffer(m_indices, sizeof(m_indices)));

    m_Layout.reset(new VertexBufferLayout());
    m_Layout->Push(2, DataType::FLOAT);  // pos
    m_Layout->Push(2, DataType::FLOAT);  // texture

    m_VA->AddBuffer(*m_VB, *m_Layout);

    m_QuadShader.reset(new Shader("./res/shader/vertex/quad.vertexShader",
        "./res/shader/fragment/quad.fragmentShader"));
    m_Texture.reset(new Texture(texturePath));
}

Quad2D::~Quad2D() {}

void Quad2D::Draw() const
{
    m_QuadShader->Bind();
    m_VA->Bind();
    m_VB->Bind();
    m_IB->Bind();
    m_Texture->Bind();

    ///
    /// Primitive(shape) we want to use
    /// How many indices we want to draw
    /// Data type of indices
    /// Index of indices
    ///
    glDrawElements(GL_TRIANGLES, m_IB->GetCount(), GL_UNSIGNED_INT, 0);
}

std::vector<glm::vec3> Quad2D::GetInitialPosition() const
{
    return m_initialPosition;
}

bool Quad2D::ClickCollision(float cursorX, float cursorY) const
{
    extern glm::mat3 mvp;

    if ((cursorX >= m_Center.x - m_width && cursorX <= m_Center.x + m_width) &&
        (cursorY >= m_Center.y - m_height &&
            cursorY <= m_Center.y + m_height))
    {
        return true;
    }
    else
        return false;
}

void Quad2D::BindShader() const { m_QuadShader->Bind(); }

void Quad2D::SetUniform1f(const std::string& name, float value) const
{
    m_QuadShader->SetUniform1f(name, value);
}

void Quad2D::SetUniform4f(const std::string& name, float v0, float v1, float v2,
    float v3) const
{
    m_QuadShader->SetUniform4f(name, v0, v1, v2, v3);
}

void Quad2D::SetUniformMat4f(const std::string& name,
    const glm::mat4& matrix) const
{
    m_QuadShader->SetUniformMat4f(name, matrix);
}

} // namespace Engine