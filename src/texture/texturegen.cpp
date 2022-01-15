#include "incs.hpp"
#include "plog/Log.h"
#include "stb_image.h"

#include "texturegen.hpp"
#include "texture.hpp"
#include "shader.hpp"

namespace {
    struct Img {
        GLubyte *source{ nullptr };
        int width{-1};
        int height{-1};
        int nrChannels{-1};
        explicit Img(const std::string &path) {
            stbi_set_flip_vertically_on_load(true);
            source = ::stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
            if(!valid()) {
                LOGE << "[Texture] Cannot load '" << path << "' image";
                return;
            }
            LOGI << "[Texture] The '" << path << "' was successfully loaded:";
            LOGI << "[Texture] Texture width: " << width;
            LOGI << "[Texture] Texture height: " << height;
            LOGI << "[Texture] Texture channels: " << nrChannels;
        }
        ~Img() {
            if(nullptr != source) {
                ::stbi_image_free(source);
                source = nullptr;
            }
        }
        bool valid() const {
            return nullptr != source;
        }
        GLenum getChannels() const {
            switch(nrChannels) {
                case 3: return GL_RGB;
                case 4: return GL_RGBA;
                default: return GL_FALSE;
            }
        }
    };
}

//== == == == == == == == == == == =  Textures  = == == == == == == == == == == ==//

class Texture2D : public Texture {
public:
    explicit Texture2D(GLenum type, GLenum position) : Texture(type, position) {}

    TextureError load(const std::string &texFilename) override {
        if(texFilename.empty()) {
            return TextureError::EmptyFilename;
        }

        Img image(texFilename);
        if(!image.valid()) {
            LOGW << "[Texture] Cannot load source from file '" << texFilename << "'";
            return TextureError::CannotLoadSource;
        }

        glTexImage2D(mType, 0, image.getChannels(), image.width, image.height,
                     0, image.getChannels(), GL_UNSIGNED_BYTE, image.source);
        glGenerateMipmap(mType);

        return TextureError::NoError;
    }
};

//== == == == == == == == == == = TextureGenerator = == == == == == == == == == ==//

Texture::Ref TextureGenerator::Gen(const std::string &filename, Shader::Ref shader, GLenum type) {
    Params defaultParams {
        std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
        std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT),
        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR),
        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    };
    return Gen(filename, shader, defaultParams, type);
}

Texture::Ref TextureGenerator::Gen(const std::string &filename, Shader::Ref shader, const Params &params, GLenum type) {
    static GLenum position = GL_TEXTURE0;
    if(GL_TEXTURE31 == position) {
        LOGW << "[TextureGenerator] Cannot create texture: the all slots was filles";
        return nullptr;
    }

    Texture::Ref texture;
    switch(type) {
        case GL_TEXTURE_2D:
            texture = std::make_shared<Texture2D>(type, position);
            break;
        default:
            LOGE << "[TextureGenerator] Incorrect type";
            return nullptr;
    }

    for(const auto &param : params) {
        texture->Set(param.first, param.second);
    }

    TextureError status{ texture->load(filename) };
    if(TextureError::NoError != status) {
        LOGE << "[TextureGenerator] The loading was failed: " << static_cast<int>(status);
    }
    int id{ static_cast<int>(position) - GL_TEXTURE0 };
    LOGI << "[TextureGenerator] The texture id: " << id;
    shader->Set("sample_" + std::to_string(id), id);
    ++position;
    return texture;
}
