#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

struct Vertex;

enum class TemplateType {
    TRIANGLE,
    SQUARE,
};

struct TemplateGenerator {
    using Template = std::pair<std::vector<Vertex>, std::vector<GLuint>>;
    static Template Generate(TemplateType type, float scale = 8.0f);
};

#endif // __TRIANGLE_H__
