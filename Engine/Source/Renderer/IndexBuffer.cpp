#include <stdint.h>

#include "Buffer.h"
#include "glad/glad.h"

namespace Engine
{
CIndexBuffer::CIndexBuffer(const uint32_t* indices, uint32_t count)
    : m_Count(count)
{
    glCreateBuffers(1, &m_RendererID);

    // QUESTION: Why are we binding to GL_ARRAY_BUFFER instead of GL_ELEMENT_ARRAY_BUFFER?
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

CIndexBuffer::~CIndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void CIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void CIndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Engine