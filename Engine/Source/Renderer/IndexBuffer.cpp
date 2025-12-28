#include <stdint.h>

#include "Buffer.h"
#include "glad/glad.h"

namespace Engine
{

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t size)
    : m_RendererID(0)
{
    glCreateBuffers(1, &m_RendererID);

    this->m_count = size / (sizeof(uint32_t));

    this->SetData(data, size);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

inline void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

inline void IndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline void IndexBuffer::SetData(const uint32_t* data, uint32_t size)
{
    this->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

uint32_t IndexBuffer::GetCount() const { return m_count; }

uint32_t IndexBuffer::GetObjectID() const { return m_RendererID; };

}  // namespace Engine