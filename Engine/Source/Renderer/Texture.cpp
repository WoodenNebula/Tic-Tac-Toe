#include "Texture.h"

#include "glad/glad.h"
#include "stb_image.h"

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

CTexture::CTexture(const std::filesystem::path& path)
    : m_FilePath(path)
{
    stbi_set_flip_vertically_on_load(1);
    auto data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_BPP, 4);
    if (data)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

        glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    else
    {
        LOG(Texture, ERROR, "Failed to load texture from path: {} -> {}", path.string(), stbi_failure_reason());
    }
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