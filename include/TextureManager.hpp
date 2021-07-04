#ifndef __TEXTURE_MANAGER_HPP__
#define __TEXTURE_MANAGER_HPP__

#include <glad/glad.h>

#include <string>

namespace util
{
class TextureManager
{
public:
    TextureManager(const std::string& i_texturePath, GLenum i_wrapParam = GL_REPEAT);

    void activate(GLenum i_texUnit);

    GLuint getId() const;
private:
    GLuint d_texId;
};

} // namespace util


#endif // __TEXTURE_MANAGER_HPP__
