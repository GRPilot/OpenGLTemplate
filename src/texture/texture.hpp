#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <memory>

enum class TextureError : int {
    NoError,
    EmptyFilename,
    CannotLoadSource,
    IncorrectType,
    IncorrectPos,
};

class Texture {
public:
    using Ref = std::shared_ptr<Texture>;

    Texture(GLenum type, GLenum position);
    virtual ~Texture();

    void Set(GLenum pname, GLint value);

    virtual TextureError load(const std::string &texFilename) = 0;
    virtual void Bind();
    virtual void Unbind();

    GLenum id() const;
    GLenum type() const;

protected:
    GLuint mId;
    GLenum mType;
    GLenum mPos;
};

#endif // __TEXTURE_H__
