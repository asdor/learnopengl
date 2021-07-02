#include "ShadersManager.hpp"

#include <iostream>
#include <fstream>

namespace
{
void checkShaderCompilation(GLuint i_shader, std::string_view i_shaderName)
{
    int success = 0;
    char infoLog[512];
    glGetShaderiv(i_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(i_shader, 512, nullptr, infoLog);
        throw std::runtime_error(std::string(i_shaderName) + " compilation failed:\n" + infoLog);
    }
}

GLuint prepareShader(std::string_view i_shaderPath, GLenum i_shaderType)
{
    std::ifstream shaderFile(i_shaderPath.data());
    if (!shaderFile)
        throw std::runtime_error("Failed to open: " + std::string(i_shaderPath));

    std::string shader;
    std::string line;
    while(std::getline(shaderFile, line))
    {
        shader += line + '\n';
    }

    unsigned int shaderId = glCreateShader(i_shaderType);
    const auto shaderCStr = shader.c_str();
    glShaderSource(shaderId, 1, &shaderCStr, nullptr);
    glCompileShader(shaderId);
    checkShaderCompilation(shaderId, i_shaderPath);

    return shaderId;
}
}

util::ShadersManager::ShadersManager(std::string_view i_vertexShaderPath, std::string_view i_fragmentShaderPath)
{
    const auto vertexId = prepareShader(i_vertexShaderPath, GL_VERTEX_SHADER);
    const auto fragmentId = prepareShader(i_fragmentShaderPath, GL_FRAGMENT_SHADER);

    d_programId = glCreateProgram();
    glAttachShader(d_programId, vertexId);
    glAttachShader(d_programId, fragmentId);
    glLinkProgram(d_programId);

    GLint success = 0;
    char infoLog[512];
    glGetProgramiv(d_programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(d_programId, 512, nullptr, infoLog);
        throw std::runtime_error("Shaider linking failed" + std::string(infoLog));
    }

    // glDeleteShader(vertexId);
    // glDeleteShader(fragmentId);
}

void util::ShadersManager::render()
{
    glUseProgram(d_programId);
}

GLuint util::ShadersManager::getId() const
{
    return d_programId;
}
