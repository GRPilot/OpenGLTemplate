#ifndef __VERTEX_H__
#define __VERTEX_H__

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 textureCoord;

    static size_t getPosOffset();
    static size_t getClrOffset();
    static size_t getTexOffset();

    static size_t getPosSize();
    static size_t getClrSize();
    static size_t getTexSize();

    static size_t getPosCount();
    static size_t getClrCount();
    static size_t getTexCount();

    static size_t getStride();
};

#endif // __VERTEX_H__
