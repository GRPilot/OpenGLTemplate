#ifndef __TEXTUREGEN_H__
#define __TEXTUREGEN_H__

class Texture;
using TexturePtr = std::shared_ptr<Texture>;
class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

struct TextureGenerator {
    using Params = std::map<GLenum, GLint>;
    static TexturePtr gen(const std::string &filename, ShaderPtr shader, GLenum type = GL_TEXTURE_2D);
    static TexturePtr gen(const std::string &filename, ShaderPtr shader, const Params &params, GLenum type = GL_TEXTURE_2D);
};

#endif // __TEXTUREGEN_H__
