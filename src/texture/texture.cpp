#include "incs.hpp"

#include "texture.hpp"

Texture::Texture(GLenum type, GLenum position)
    : mId{}, mType{ type }, mPos{ position }
{
    glGenTextures(1, &mId);
    Bind();
}

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}

void Texture::Set(GLenum pname, GLint value) {
    glTextureParameteri(mType, pname, value);
}

void Texture::Bind() {
    glActiveTexture(mPos);
    glBindTexture(mType, mId);
}

void Texture::Unbind() {
    glActiveTexture(0);
    glBindTexture(mType, 0);
}

GLenum Texture::id() const {
    return mId;
}

GLenum Texture::type() const {
    return mType;
}

