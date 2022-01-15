#ifndef __MASH_H__
#define __MASH_H__

struct Vertex;
class Shader;

class Mash {
public:
    explicit Mash(const std::vector<Vertex> &vertices,
                  const std::vector<GLuint> &indices,
                  std::shared_ptr<Shader> &mash_shader);
    ~Mash();

    void bind();
    void unbind();
    void draw();

private:
    size_t draw_count;
    std::shared_ptr<Shader> shader;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    bool argsValid(const std::vector<Vertex> &vertices,
                        const std::vector<GLuint> &indices,
                        std::shared_ptr<Shader> &mash_shader) const;
};

#endif // __MASH_H__
