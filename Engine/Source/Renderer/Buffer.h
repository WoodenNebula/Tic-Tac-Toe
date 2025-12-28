#pragma once
#include <stdint.h>

namespace Engine
{

class VertexBuffer
{
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    uint32_t GetObjectID() const;

    void Bind() const;
    void UnBind() const;

    /// <summary>
    /// Copies the provided array of vertices to the Buffer
    /// </summary>
    /// <param name="data">pointer to the array of vertices</param>
    /// <param name="size">size of the array data (size)</param>
    void SetData(const void* data, const uint32_t size);

private:
    uint32_t m_RendererID;
};

class IndexBuffer
{
public:
    IndexBuffer(const uint32_t* data, uint32_t size);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    /// <summary>
    /// Copies the provided array of indices to the Buffer
    /// Also it automatically binds the IBO associated with it before copying
    /// the data
    ///
    /// </summary>
    /// <param name="data">pointer to the array of indices</param>
    /// <param name="size">size of the element buffer array</param>
    void SetData(const uint32_t* data, uint32_t size);

    uint32_t GetCount() const;

    uint32_t GetObjectID() const;

private:
    uint32_t m_RendererID, m_count;
};
}; // namespace Engine