#pragma once

#include "Renderer/Buffer.h"
#include <memory>

namespace Engine
{
class CVertexArray
{
public:
    CVertexArray();
    ~CVertexArray();

    void Bind() const;
    void UnBind() const;

    void AddVertexBuffer(const std::shared_ptr<CVertexBuffer>& VertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<CIndexBuffer>& IndexBuffer);

    const std::vector<std::shared_ptr<CVertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
    const std::shared_ptr<CIndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex{ 0 };
    std::vector<std::shared_ptr<CVertexBuffer>> m_VertexBuffers;
    std::shared_ptr<CIndexBuffer> m_IndexBuffer;
};
};