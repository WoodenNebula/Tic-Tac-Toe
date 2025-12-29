#include "Shader.h"

#include "Core/CustomAssert.h"
#include <memory.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"

namespace Engine
{
CShader::CShader(const std::filesystem::path& vertexFilePath, const std::filesystem::path& fragmentFilePath)
    : m_VertexFilePath(vertexFilePath),
    m_FragmentFilePath(fragmentFilePath),
    m_ProgramID(0)
{
    std::string vertexSrc = ParseShader(vertexFilePath);
    std::string fragmentSrc = ParseShader(fragmentFilePath);

    m_ProgramID = CreateShaderProgram(vertexSrc, fragmentSrc);
}

void CShader::Bind() const { glUseProgram(m_ProgramID); }

void CShader::UnBind() const { glUseProgram(0); }

uint32_t CShader::GetProgramID() { return m_ProgramID; }

std::string CShader::ParseShader(const std::filesystem::path& filePath)
{
    std::fstream shaderFile(filePath);
    std::string line;
    std::stringstream ss;

    // Ensure that shader file is open
    if (!shaderFile.is_open())
    {
        std::cout << "ERROR::SHADER::FILE::READ -> Couldn't open shader file!\n"
            << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(shaderFile, line))
    {
        ss << line.c_str() << "\n";
    }

    return ss.str();
}

/* creates a shader program that has to be attached and linked */
uint32_t CShader::CreateShaderProgram(const std::string& vertexShaderSrc,
    const std::string& fragmentShaderSrc)
{
    uint32_t vertexShader, fragmentShader;
    vertexShader = CompileShader(ShaderType::VERTEX, vertexShaderSrc);
    fragmentShader = CompileShader(ShaderType::FRAGMENT, fragmentShaderSrc);

    uint32_t shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Error Checking
    int isLinked = 0;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        int length = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        std::string err;
        err.reserve(length + 1);


        char* infoLog = (char*)malloc((length + 1) * sizeof(char));
        glGetShaderInfoLog(shaderProgram, length, &length, err.data());

        std::cout << "ERROR::SHADER : LINKING FAILED \n"
            << err << std::endl;

        glDeleteShader(shaderProgram);

        return 0;
    }

    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

uint32_t CShader::CompileShader(ShaderType type, const std::string& shaderSrc)
{
/// Compilation Process
    uint32_t shaderID = 0;
    const char* src = shaderSrc.c_str();
    if (type == ShaderType::VERTEX)
    {
        shaderID = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == ShaderType::FRAGMENT)
    {
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        std::cout << "ERROR::SHADER::BadShaderType" << std::endl;
        exit(EXIT_FAILURE);
    }
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    // Error Handling
    int isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled)
    {
        int length = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

        char* infoLog = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(shaderID, length, &length, infoLog);

        std::cout << "ERROR::SHADER::"
            << (type == ShaderType::VERTEX ? "VERTEX" : "FRAGMENT")
            << "::COMPILATION_FAILED\n"
            << infoLog << std::endl;

        glDeleteShader(shaderID);

        return 0;
    }

    return shaderID;
}

void CShader::SetUniform1i(const std::string& name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void CShader::SetUniform1f(const std::string& name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void CShader::SetUniform4f(const std::string& name, float v0, float v1, float v2,
    float v3) const
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void CShader::SetUniformMat4f(const std::string& name,
    const glm::mat4& matrix) const
{
// transpose is necessary if matrix is row major
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

uint32_t CShader::GetUniformLocation(const std::string& name) const
{
// check if the uniform already exists and if it does, return its location
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1)
    {
        std::cout << "SHADER::UNIFORM -> Uniform " << name << " Doesn't Exist"
            << std::endl;
        ASSERT(false);
    }

    m_UniformLocationCache[name] = location;
    return location;
}

} // namespace Engine