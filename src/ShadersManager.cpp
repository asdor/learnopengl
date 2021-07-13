#include "ShadersManager.hpp"

#include <glm/gtc/type_ptr.hpp>

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

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

void util::ShadersManager::render()
{
    glUseProgram(d_programId);
}

GLuint util::ShadersManager::getId() const
{
    return d_programId;
}

void util::ShadersManager::setBool(const std::string& i_name, bool i_value) const
{
    setInt(i_name, static_cast<int>(i_value));
}

void util::ShadersManager::setInt(const std::string& i_name, int i_value) const
{
    glUniform1i(glGetUniformLocation(d_programId, i_name.c_str()), i_value);
}

void util::ShadersManager::setFloat(const std::string& i_name, float i_value) const
{
    glUniform1f(glGetUniformLocation(d_programId, i_name.c_str()), i_value);
}

void util::ShadersManager::setVec3(const std::string& i_name, const glm::vec3& i_vec) const
{
    const int vecLoc = glGetUniformLocation(d_programId, i_name.c_str());
    glUniform3fv(vecLoc, 1, glm::value_ptr(i_vec));
}

void util::ShadersManager::setMatrix4fv(const std::string& i_name, const glm::mat4& i_matrix) const
{
    const int matrixLoc = glGetUniformLocation(d_programId, i_name.c_str());
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, glm::value_ptr(i_matrix));
}
