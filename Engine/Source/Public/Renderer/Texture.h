#pragma once
#include <filesystem>

#include "AssetManager/AssetManager.h"

namespace Engine
{

class CTexture
{
public:
    CTexture();
    CTexture(const FPath& path);
    ~CTexture();

    void Bind(uint32_t slot = 0) const;
    void UnBind() const;

    void SetData(void* data, uint32_t size);

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Width; }
    inline int GetBytesPerPixel() const { return m_BPP; }
private:
    uint32_t m_RendererID{ 0 };
    FPath m_FilePath;
    int m_Width{ 0 }, m_Height{ 0 };
    // m_BPP = BytesPerPixel
    int m_BPP{ 4 };
};
} // namespace Engine