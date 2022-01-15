#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 textureCoord;

    static size_t GetPosOffset();
    static size_t GetClrOffset();
    static size_t GetTexOffset();

    static size_t GetPosSize();
    static size_t GetClrSize();
    static size_t GetTexSize();

    static size_t GetPosCount();
    static size_t GetClrCount();
    static size_t GetTexCount();

    static size_t GetStride();
};

using Vertices = std::vector<Vertex>;

#endif // __VERTEX_H__
