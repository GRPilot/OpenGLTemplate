#include "incs.hpp"

#include "texture.hpp"

Texture::Texture(GLenum type, GLenum position) : ID(0), TYPE(type), POS(position) {
    glGenTextures(1, &ID);
    bind();
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::set(GLenum pname, GLint value) {
    glTextureParameteri(TYPE, pname, value);
}

void Texture::bind() {
    glActiveTexture(POS);
    glBindTexture(TYPE, ID);
}

void Texture::unbind() {
    glActiveTexture(0);
    glBindTexture(TYPE, 0);
}

GLenum Texture::id() const {
    return ID;
}

GLenum Texture::type() const {
    return TYPE;
}

