#include "incs.hpp"
#include "mash.hpp"
#include "plog/Log.h"

#include "vertex.hpp"
#include "shader.hpp"

namespace {
    template<class T> inline GLsizei getLen(const std::vector<T> &vec) {
        return vec.size() * sizeof(T);
    }
    // inline void *asVoidptr(size_t &&number) {
        // return static_cast<void *>(&number);
    // }
    #define asVoidptr(n) (void *)(n)

    void printinfo(const std::string &prop, int id, size_t cnt, size_t off, size_t stride) {
        LOGI << "[Mash] " << std::string(60, '-');
        LOGI << "[Mash] Property: " << prop;
        LOGI << "[Mash]   id: " << id;
        LOGI << "[Mash]   count: " << cnt;
        LOGI << "[Mash]   offset: " << off;
        LOGI << "[Mash]   stride: " << stride;
    }
}

Mash::Mash(const std::vector<Vertex> &vertices,
           const std::vector<GLuint> &indices,
           std::shared_ptr<Shader> &mash_shader)
    : draw_count(indices.size()), shader(mash_shader) {
    if(!argsValid(vertices, indices, mash_shader)) {
        LOGE << "[Mash] The arguments is not valid";
        return;
    }
    LOGI << "[Mash] draw_count: " << draw_count;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    shader->use();

    glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, getLen(vertices), &vertices[0], GL_STATIC_DRAW);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLen(indices), &indices[0], GL_STATIC_DRAW);

     int pos_id = shader->location("position", Shader::PropertyType::ATTRIBUTE);
     glVertexAttribPointer(pos_id, Vertex::getPosCount(), GL_FLOAT, GL_FALSE,
                           Vertex::getStride(), asVoidptr(Vertex::getPosOffset()));
     glEnableVertexAttribArray(pos_id);

     int clr_id = shader->location("color", Shader::PropertyType::ATTRIBUTE);
     glVertexAttribPointer(clr_id, Vertex::getClrCount(), GL_FLOAT, GL_FALSE,
                           Vertex::getStride(), asVoidptr(Vertex::getClrOffset()));
     glEnableVertexAttribArray(clr_id);

     int tex_id = shader->location("texCoord", Shader::PropertyType::ATTRIBUTE);
     glVertexAttribPointer(tex_id, Vertex::getTexCount(), GL_FLOAT, GL_FALSE,
                           Vertex::getStride(), asVoidptr(Vertex::getTexOffset()));
     glEnableVertexAttribArray(tex_id);
     
    glBindVertexArray(0);

    // log what we build
    printinfo("position", pos_id, Vertex::getPosCount(), Vertex::getPosOffset(), Vertex::getStride());
    printinfo("color", clr_id, Vertex::getClrCount(), Vertex::getClrOffset(), Vertex::getStride());
    printinfo("texCoord", tex_id, Vertex::getTexCount(), Vertex::getTexOffset(), Vertex::getStride());

}

Mash::~Mash() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mash::bind() {
    shader->use();
    glBindVertexArray(VAO);
}
void Mash::unbind() {
    glBindVertexArray(0);
    shader->unuse();
}

void Mash::draw() {
    glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, nullptr);
}

bool Mash::argsValid(const std::vector<Vertex> &vertices,
                     const std::vector<GLuint> &indices,
                     std::shared_ptr<Shader> &mash_shader) const {
    bool valid = true;
    if(vertices.empty()) {
        valid = false;
        LOGE << "[Mash] The vertices array is empty";
    }
    if(indices.empty()) {
        valid = false;
        LOGE << "[Mash] The indices array is empty";
    }
    if(nullptr == mash_shader) {
        valid = false;
        LOGE << "[Mash] The mash shader is null";
    }

    return valid;
}
