#pragma once

#include <vector>


#include "Core/CustomAssert.h"
#include "glad/glad.h"

namespace Engine
{


struct VertexBufferElement
{
    uint32_t type;
    uint32_t count;
    uint8_t normalized;

    static uint32_t GetSizeOfType(uint32_t type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false);
        return 0;
    }
};

enum class DataType { FLOAT = 0, U_INT, U_CHAR };

class VertexBufferLayout
{

public:
    VertexBufferLayout() : m_Stride(0) {};
    ~VertexBufferLayout() {};

    /// <param name="count">:number of elements per vertex attrib. For more than
    /// one attrib, this function has to be called each time with the count of
    /// elements in each attrib separately</param>
    void Push(uint32_t count, DataType type)
    {
        switch (type)
        {
        case DataType::FLOAT:
            m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
            m_Stride +=
                count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
            break;

        case DataType::U_INT:
            m_Stride +=
                count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
            m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });

        case DataType::U_CHAR:
            m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
            m_Stride += count * VertexBufferElement::GetSizeOfType(
                GL_UNSIGNED_BYTE);
        default:
            ASSERT(false);
            break;
        }
    }

    inline const std::vector<VertexBufferElement>& GetElements() const
    {
        return m_Elements;
    }

    inline uint32_t GetStride() const { return m_Stride; }
private:
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;
};
} // namespace Engine