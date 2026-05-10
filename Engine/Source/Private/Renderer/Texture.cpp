#include "Renderer/Texture.h"

#include "AssetManager/Image.h"
#include "glad/glad.h"

#include "Logger/Logger.h"
#include <filesystem>

namespace Engine
{
DECLARE_LOG_CATEGORY(Texture)

CTexture::CTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

CTexture::CTexture(const FPath& path)
    : m_FilePath(path)
{

    CImage img(path);
    SGenericError err = img.Load();
    if (err)
    {
        LOG(Texture, ERROR, "Failed to load texture: ", err);
    }
    m_Width = img.Dimensions.x;
    m_Height = img.Dimensions.y;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

    glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, img.Data);
}

CTexture::~CTexture() { glDeleteTextures(1, &m_RendererID); }

void CTexture::SetData(void* data, uint32_t size)
{
    uint32_t bpp = 4; // RGBA

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}


void CTexture::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}

void CTexture::UnBind() const { glBindTexture(GL_TEXTURE_2D, 0); }
} // namespace Engine
