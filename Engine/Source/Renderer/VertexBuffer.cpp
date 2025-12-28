#include "Buffer.h"
#include "glad/glad.h"

namespace Engine
{
VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
    glCreateBuffers(1, &m_RendererID);
    this->SetData(data, size);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

uint32_t VertexBuffer::GetObjectID() const { return m_RendererID; }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexBuffer::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::SetData(const void* data, const uint32_t size)
{
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
}; // namespace Engine