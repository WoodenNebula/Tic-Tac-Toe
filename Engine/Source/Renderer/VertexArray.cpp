#include "VertexArray.h"

#include "Buffer.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

namespace Engine
{
CVertexArray::CVertexArray()
{
    glCreateVertexArrays(1, &m_RendererID);
}

CVertexArray::~CVertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void CVertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void CVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void CVertexArray::AddVertexBuffer(const std::shared_ptr<CVertexBuffer>& VertexBuffer)
{
    this->Bind();
    VertexBuffer->Bind();

    const CVertexBufferLayout& layout = VertexBuffer->GetLayout();
    const std::vector<VertexBufferElement>& elements = layout.GetElements();
    uint32_t offset = 0;
    for (const auto& element : elements)
    {
        glEnableVertexAttribArray(m_VertexBufferIndex);
        glVertexAttribPointer(m_VertexBufferIndex, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        m_VertexBufferIndex++;
    }

    m_VertexBuffers.push_back(VertexBuffer);
}

void CVertexArray::SetIndexBuffer(const std::shared_ptr<CIndexBuffer>& IndexBuffer)
{
    Bind();
    IndexBuffer->Bind();

    m_IndexBuffer = IndexBuffer;
}

}   // namespace Engine