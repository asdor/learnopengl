#include "Texture.hpp"

#include <stb_image.h>

#include <stdexcept>
#include <unordered_map>

namespace
{
GLenum channelsToFormat(int i_channelsNum)
{
    switch(i_channelsNum)
    {
    case 1:
        return GL_RED;
    case 3:
        return GL_RGB;
    case 4:
        return GL_RGBA;
    default:
        return GL_RED;
    }
}
}

utils::Texture::Texture(const std::string& i_texturePath, aiTextureType i_textureType, GLenum i_wrapParam /* = GL_REPEAT */) : d_texId(0), d_textureType(i_textureType)
{
    int texWidth = 0;
    int texHeight = 0;
    int texNrChannels = 0;
    unsigned char* texData = stbi_load(i_texturePath.c_str(), &texWidth, &texHeight, &texNrChannels, 0);
    if (!texData)
        throw std::runtime_error("Failed to load texture: " + i_texturePath);

    glGenTextures(1, &d_texId);
    glBindTexture(GL_TEXTURE_2D, d_texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, i_wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, i_wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto imageFormat = channelsToFormat(texNrChannels);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, texWidth, texHeight, 0, imageFormat, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(texData);
}

utils::Texture::Texture(const std::string& i_texturePath, GLenum i_wrapParam /* = GL_REPEAT */) : Texture(i_texturePath, aiTextureType::aiTextureType_UNKNOWN, i_wrapParam)
{

}

void utils::Texture::activate(GLenum i_texUnit) const
{
    glActiveTexture(i_texUnit);
    glBindTexture(GL_TEXTURE_2D, d_texId);
}

GLuint utils::Texture::getId() const
{
    return d_texId;
}

aiTextureType utils::Texture::getType() const
{
    return d_textureType;
}

std::string utils::Texture::getTypeAsString() const
{
    static const std::unordered_map<aiTextureType, std::string> typeToString = {
        {aiTextureType::aiTextureType_DIFFUSE, "diffuse"},
        {aiTextureType::aiTextureType_SPECULAR, "specular"}
    };

    auto it = typeToString.find(d_textureType);
    if (it == typeToString.end())
        throw std::runtime_error("Bad texture type: " + static_cast<int>(d_textureType));
    return it->second;
}
