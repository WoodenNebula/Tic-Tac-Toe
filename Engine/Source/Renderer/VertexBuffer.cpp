#include "Buffer.h"
#include "glad/glad.h"

namespace Engine
{
CVertexBuffer::CVertexBuffer(uint32_t size)
{
    glCreateBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
}

CVertexBuffer::CVertexBuffer(const float* vertices, uint32_t size)
{
    glCreateBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

CVertexBuffer::~CVertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void CVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void CVertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::SetData(const void* data, const uint32_t size)
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
}; // namespace Engine