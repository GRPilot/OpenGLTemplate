#include "incs.hpp"
#include "plog/Log.h"

#include "shader.hpp"

namespace {
    void printSource(std::string src) {
        if(src.back() == '\n') {
            src.pop_back();
        }

        size_t line = 0;
        std::string strline;
        for(const char c : src) {
            if('\n' == c) {
                LOGI << "[Shader] " << ++line << " |" << strline;
                strline.clear();
                continue;
            }
            strline += c;
        }
        LOGI << "[Shader] " << ++line << " |" << strline;
    }
}

Shader::Shader(const std::string &vShader, const std::string &fShader)
    : prog{ glCreateProgram() }, lastError{ nullptr }
{
    if(vShader.empty() || fShader.empty()) {
        lastError.reset(new ShaderError{"The pathes to shaders is empty.", -1});
        return;
    }
    auto onError = [&](GLuint id) {
        const auto err{ getError(id, GL_SHADER) };
        LOGE << "[Shader] " << err;
        lastError.reset(new ShaderError{err, -2});
    };

    GLuint vertex{ generateShader(GL_VERTEX_SHADER, vShader) };
    if(hasError(vertex, GL_COMPILE_STATUS)) {
        onError(vertex);
    }
    GLuint fragment = generateShader(GL_FRAGMENT_SHADER, fShader);
    if(hasError(fragment, GL_COMPILE_STATUS)) {
        onError(vertex);
    }

    glAttachShader(prog, vertex);
    glAttachShader(prog, fragment);
    glLinkProgram(prog);
    if(hasError(prog, GL_LINK_STATUS)) {
        onError(prog);
    }
}

Shader::~Shader() {
    glDeleteProgram(prog);
}

bool Shader::Valide() const {
    return !static_cast<bool>(lastError);
}

std::shared_ptr<ShaderError> Shader::GetLastError() const {
    return lastError;
}

int Shader::Location(const std::string &property, PropertyType type) const {
    GLint loc{ -1 };
    switch(type) {
        case PropertyType::UNIFORM:
            loc = glGetUniformLocation(prog, property.c_str());
            break;
        case PropertyType::ATTRIBUTE:
            loc = glGetAttribLocation(prog, property.c_str());
            break;
    }
    return loc;
}

void Shader::Set(const std::string &property, float value) {
    glUniform1f(this->Location(property), value);
}

void Shader::Set(const std::string &property, int value) {
    glUniform1i(this->Location(property), value);
}

void Shader::Set(const std::string &property, bool value) {
    glUniform1i(this->Location(property), static_cast<int>(value));
}

void Shader::Set(const std::string &property, const glm::mat4 &value) {
    glUniformMatrix4fv(this->Location(property), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::Use() {
    glUseProgram(prog);
}

void Shader::UnUse() {
    glUseProgram(0);
}

GLuint Shader::generateShader(GLenum type, const std::string &fpath) const {
    auto source{ loadFormFile(fpath) };
    if(source.empty()) {
        return 0;
    }
    LOGI << "[Shader] Load source from '" << fpath << "'";
    printSource(source);
    GLuint shader{ glCreateShader(type) };
    const char *src{ source.c_str() };
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    LOGI << "[Shader] Generated shader: " << shader;
    return shader;
}

std::string Shader::loadFormFile(const std::string &fname) const try {
    std::ifstream file;
    file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    file.open(fname);
    std::stringstream strstream;
    strstream << file.rdbuf();
    return strstream.str();
} catch (std::fstream::failure &fail) {
    LOGE << "[Shader] Cannot load '" << fname << "' file: " << fail.what();
    lastError.reset(new ShaderError{fail.what(), fail.code().value()});
    return "";
}

bool Shader::hasError(GLuint target, GLenum what) const {
    if(target == 0) {
        return false;
    }
    int success{};
    if(GL_LINK_STATUS == what) {
        glGetProgramiv(target, what, &success);
    } else {
        glGetShaderiv(target, what, &success);
    }
    return GL_TRUE != success;
}

std::string Shader::getError(GLuint target, GLenum type) const {
    if(target == 0) {
        return "Failed while creating";
    }
    char error[2048]{};
    memset(error, 0, sizeof(error));
    GLsizei errsize{};
    if(GL_PROGRAM == type) {
        glGetProgramInfoLog(target, sizeof(error), &errsize, error);
    } else {
        glGetShaderInfoLog(target, sizeof(error), &errsize, error);
    }
    if(nullptr == error || errsize <= 0) {
        return "";
    }

    return std::string(error, errsize);
}
