#include "TextureManager.hpp"

#include <stb_image.h>

#include <stdexcept>

util::TextureManager::TextureManager(const std::string& i_texturePath, GLenum i_wrapParam)
{
    int texWidth = 0;
    int texHeight = 0;
    int texNrChannels = 0;
    unsigned char* texData = stbi_load(i_texturePath.c_str(), &texWidth, &texHeight, &texNrChannels, 0);
    if (!texData)
        throw std::runtime_error("Failed to load texture: " + i_texturePath);

    d_texId = 0;
    glGenTextures(1, &d_texId);
    glBindTexture(GL_TEXTURE_2D, d_texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, i_wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, i_wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const auto imageFormat = i_texturePath.ends_with("png") ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, texWidth, texHeight, 0, imageFormat, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texData);
}

void util::TextureManager::activate(GLenum i_texUnit)
{
    glActiveTexture(i_texUnit);
    glBindTexture(GL_TEXTURE_2D, d_texId);
}


GLuint util::TextureManager::getId() const
{
    return d_texId;
}
