#ifndef __TEXTUREGEN_H__
#define __TEXTUREGEN_H__

#include "texture.hpp"
#include "shader.hpp"

struct TextureGenerator {
    using Params = std::map<GLenum, GLint>;
    static Texture::Ref Gen(const std::string &filename, Shader::Ref shader, GLenum type = GL_TEXTURE_2D);
    static Texture::Ref Gen(const std::string &filename, Shader::Ref shader, const Params &params, GLenum type = GL_TEXTURE_2D);
};

#endif // __TEXTUREGEN_H__
