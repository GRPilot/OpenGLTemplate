#ifndef __SHADER_H__
#define __SHADER_H__

#include <memory>

struct ShaderError {
    using Ref = std::shared_ptr<ShaderError>;
    const std::string what;
    const int code;
};

class Shader {
public:
    using Ref = std::shared_ptr<Shader>;

    Shader(const std::string &vShader, const std::string &fShader);
    virtual ~Shader();

    bool Valide() const;
    ShaderError::Ref GetLastError() const;

    enum class PropertyType { UNIFORM, ATTRIBUTE };
    int Location(const std::string &property, PropertyType type = PropertyType::UNIFORM) const;

    void Set(const std::string &property, float value);
    void Set(const std::string &property, int value);
    void Set(const std::string &property, bool value);
    void Set(const std::string &property, const glm::mat4 &value);

    void Use();
    void UnUse();

protected:
    GLuint prog;
    mutable ShaderError::Ref lastError;

    GLuint generateShader(GLenum type, const std::string &fpath) const;
    std::string loadFormFile(const std::string &fname) const;
    bool hasError(GLuint target, GLenum what) const;
    std::string getError(GLuint target, GLenum type) const;
};

#endif // __SHADER_H__
