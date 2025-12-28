#include "VertexArray.h"

#include "Buffer.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

namespace Engine
{

VertexArray::VertexArray(uint32_t size) : m_RendererID(0)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexArray::VertexArray(float* vertices, uint32_t size)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexArray::~VertexArray() { glDeleteBuffers(1, &m_RendererID); }

void VertexArray::SetData(const void* data, uint32_t size)
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexArray::AddBuffer(const VertexBuffer& vb,
    const VertexBufferLayout& layout)
{
    this->Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type,
            element.normalized, layout.GetStride(),
            (void*)offset);

        offset +=
            element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexArray::UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
}   // namespace Engine