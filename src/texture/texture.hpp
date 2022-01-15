#ifndef __TEXTURE_H__
#define __TEXTURE_H__

enum class TextureError : int {
    NoError,
    EmptyFilename,
    CannotLoadSource,
    IncorrectType,
    IncorrectPos,
};

class Texture {
public:
    explicit Texture(GLenum type, GLenum position);
    virtual ~Texture();

    void set(GLenum pname, GLint value);

    virtual TextureError load(const std::string &texFilename) = 0;
    virtual void bind();
    virtual void unbind();

    GLenum id() const;
    GLenum type() const;

protected:
    GLuint ID;
    GLenum TYPE;
    GLenum POS;
};

#endif // __TEXTURE_H__
