#pragma once

#include "VertexBufferLayout.h"
namespace Engine
{
// Forward declarations to avoid including the headers
class VertexBuffer;

class VertexArray
{
public:
    VertexArray(uint32_t size);
    VertexArray(float* vertices, uint32_t size);
    ~VertexArray();

    void SetData(const void* data, uint32_t size);
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void UnBind() const;
private:
    uint32_t m_RendererID;
};
};