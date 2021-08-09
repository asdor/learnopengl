#ifndef __TEXTURE_MANAGER_HPP__
#define __TEXTURE_MANAGER_HPP__

#include <glad/glad.h>

#include <assimp/material.h>

#include <string>
#include <string_view>

namespace utils
{
class Texture
{
public:
    Texture() = delete;
    Texture(const std::string& i_texturePath, aiTextureType i_textureType, GLenum i_wrapParam = GL_REPEAT);
    Texture(const std::string& i_texturePath, GLenum i_wrapParam);

    void activate(GLenum i_texUnit) const;

    GLuint getId() const;
    aiTextureType getType() const;
    std::string getTypeAsString() const;

private:
    GLuint d_texId;
    aiTextureType d_textureType;
};



} // namespace utils


#endif // __TEXTURE_MANAGER_HPP__
