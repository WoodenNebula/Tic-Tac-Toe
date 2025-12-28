#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <glm.hpp>

namespace Engine
{

enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

class Shader
{
public:

    Shader(const std::filesystem::path& vertexFilePath, const std::filesystem::path& fragmentFilePath);
    uint32_t GetProgramID();

    void Bind() const;
    void UnBind() const;

    // Uniforms
    void SetUniform1i(const std::string& name, int v0) const;
    void SetUniform1f(const std::string& name, float value) const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2,
        float v3) const;
    void SetUniformMat4f(const std::string& name,
        const glm::mat4& matrix) const;

    uint32_t GetUniformLocation(const std::string& name) const;

private:
    std::string ParseShader(const std::filesystem::path& filePath);
    uint32_t CreateShaderProgram(const std::string& vertexShaderSrc,
        const std::string& fragmentShaderSrc);
    uint32_t CompileShader(ShaderType type, const std::string& shaderSrc);

private:
    uint32_t m_ProgramID;
    std::filesystem::path m_VertexFilePath;
    std::filesystem::path m_FragmentFilePath;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
} // namespace Engine