#pragma once
#include <stdint.h>
#include "Renderer/VertexBufferLayout.h"

namespace Engine
{

class CVertexBuffer
{
public:
    CVertexBuffer(uint32_t size);
    CVertexBuffer(const float* vertices, uint32_t size);
    ~CVertexBuffer();

    void Bind() const;
    void UnBind() const;

    /// <summary>
    /// Copies the provided array of vertices to the Buffer
    /// </summary>
    /// <param name="data">pointer to the array of vertices</param>
    /// <param name="size">size of the array data (size)</param>
    void SetData(const void* data, uint32_t size);

    const CVertexBufferLayout& GetLayout() const { return m_Layout; }
    void SetLayout(const CVertexBufferLayout& layout) { m_Layout = layout; }

private:
    uint32_t m_RendererID;
    CVertexBufferLayout m_Layout;
};

class CIndexBuffer
{
public:
    CIndexBuffer(const uint32_t* indices, uint32_t count);
    ~CIndexBuffer();

    void Bind() const;
    void UnBind() const;

    uint32_t GetCount() const { return m_Count; }
private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
}; // namespace Engine