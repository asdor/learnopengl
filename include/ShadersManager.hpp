#ifndef __SHADERS_MANAGER_HPP__
#define __SHADERS_MANAGER_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string_view>

namespace utils
{
class ShadersManager
{
public:
    ShadersManager(std::string_view i_vertexShaderPath, std::string_view i_fragmentShaderPath);

    void render() const;

    GLuint getId() const;

    void setBool(const std::string& i_name, bool i_value) const;
    void setInt(const std::string& i_name, int i_value) const;
    void setFloat(const std::string& i_name, float i_value) const;
    void setVec3(const std::string& i_name, const glm::vec3& i_vec) const;
    void setMatrix4fv(const std::string& i_name, const glm::mat4& i_matrix) const;

private:
    GLuint d_programId;

};
} // namespace utils

#endif // __SHADERS_MANAGER_HPP__
