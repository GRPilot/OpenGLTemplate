#ifndef __MASH_H__
#define __MASH_H__

#include "shader.hpp"
#include "vertex.hpp"

struct Vertex;
class Shader;

class Mash {
public:
    Mash(const Vertices &vertices, const std::vector<GLuint> &indices, Shader::Ref mashShader);
    ~Mash();

    void Bind();
    void Unbind();
    void Draw();

private:
    size_t mDrawCount;
    Shader::Ref shader;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    bool ArgsValid(const Vertices &vertices, const std::vector<GLuint> &indices,
                Shader::Ref &mashShader) const;
};

#endif // __MASH_H__
