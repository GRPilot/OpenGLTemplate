#ifndef __SHADER_H__
#define __SHADER_H__

struct ShaderError {
    const std::string what;
    const int code;
};

class Shader {
public:
    explicit Shader(const std::string &vShader, const std::string &fShader);
    virtual ~Shader();

    bool success() const;
    std::shared_ptr<ShaderError> getLastError() const;

    enum class PropertyType { UNIFORM, ATTRIBUTE };
    int location(const std::string &property, PropertyType type = PropertyType::UNIFORM) const;

    void set(const std::string &property, float value);
    void set(const std::string &property, int value);
    void set(const std::string &property, bool value);
    void set(const std::string &property, const glm::mat4 &value);

    void use();
    void unuse();

protected:
    GLuint prog;
    mutable std::shared_ptr<ShaderError> lastError;

    GLuint generateShader(GLenum type, const std::string &fpath) const;
    std::string loadFormFile(const std::string &fname) const;
    bool hasError(GLuint target, GLenum what) const;
    std::string getError(GLuint target, GLenum type) const;
};

#endif // __SHADER_H__
