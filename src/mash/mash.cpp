#include "incs.hpp"
#include "mash.hpp"
#include "plog/Log.h"

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

Mash::Mash(const Vertices &vertices, const std::vector<GLuint> &indices, Shader::Ref mashShader)
    : mDrawCount{ indices.size() }, shader{ mashShader }
{
    if(!ArgsValid(vertices, indices, mashShader)) {
        LOGE << "[Mash] The arguments is not valid";
        return;
    }
    LOGI << "[Mash] mDrawCount: " << mDrawCount;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    shader->Use();

    glBindVertexArray(VAO);

     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     glBufferData(GL_ARRAY_BUFFER, getLen(vertices), &vertices[0], GL_STATIC_DRAW);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLen(indices), &indices[0], GL_STATIC_DRAW);

     int pos_id{ shader->Location("position", Shader::PropertyType::ATTRIBUTE) };
     glVertexAttribPointer(pos_id, Vertex::GetPosCount(), GL_FLOAT, GL_FALSE,
                           Vertex::GetStride(), asVoidptr(Vertex::GetPosOffset()));
     glEnableVertexAttribArray(pos_id);

     int clr_id{ shader->Location("color", Shader::PropertyType::ATTRIBUTE) };
     glVertexAttribPointer(clr_id, Vertex::GetClrCount(), GL_FLOAT, GL_FALSE,
                           Vertex::GetStride(), asVoidptr(Vertex::GetClrOffset()));
     glEnableVertexAttribArray(clr_id);

     int tex_id{ shader->Location("texCoord", Shader::PropertyType::ATTRIBUTE) };
     glVertexAttribPointer(tex_id, Vertex::GetTexCount(), GL_FLOAT, GL_FALSE,
                           Vertex::GetStride(), asVoidptr(Vertex::GetTexOffset()));
     glEnableVertexAttribArray(tex_id);
     
    glBindVertexArray(0);

    // log what we build
    printinfo("position", pos_id, Vertex::GetPosCount(), Vertex::GetPosOffset(), Vertex::GetStride());
    printinfo("color", clr_id, Vertex::GetClrCount(), Vertex::GetClrOffset(), Vertex::GetStride());
    printinfo("texCoord", tex_id, Vertex::GetTexCount(), Vertex::GetTexOffset(), Vertex::GetStride());

}

Mash::~Mash() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mash::Bind() {
    shader->Use();
    glBindVertexArray(VAO);
}
void Mash::Unbind() {
    glBindVertexArray(0);
    shader->UnUse();
}

void Mash::Draw() {
    glDrawElements(GL_TRIANGLES, mDrawCount, GL_UNSIGNED_INT, nullptr);
}

bool Mash::ArgsValid(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
                     std::shared_ptr<Shader> &mash_shader) const {
    bool valid{ true };
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
