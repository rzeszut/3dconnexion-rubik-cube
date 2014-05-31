#ifndef TEXTURE_HPP_SIHZOJQQ
#define TEXTURE_HPP_SIHZOJQQ

#include <memory>

#include "GL/glew.h"
#include "loader.hpp"

namespace gl {
namespace texture {

enum class TextureFilter {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class TextureWrap {
    CLAMP_TO_EDGE,
    REPEAT,
    MIRRORED_REPEAT
};

class Texture {
    GLuint id;

    Texture(GLuint id);

public:
    ~Texture();

    Texture() = delete;
    Texture(const Texture &) = delete;
    Texture &operator =(const Texture &) = delete;

    GLuint getID() const;

    void setMinificationFilter(TextureFilter minFilter);
    void setMagnificationFilter(TextureFilter magFilter);
    void setWrap(TextureWrap wrapS, TextureWrap wrapT);

    static std::unique_ptr<Texture> fromBMPFile(const char *name);
    static std::unique_ptr<Texture> fromDDSFile(const char *name);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Texture::Texture(GLuint i) :
    id(i)
{}

inline Texture::~Texture() {
    glDeleteTextures(1, &id);
}

inline GLuint Texture::getID() const {
    return id;
}

inline std::unique_ptr<Texture> Texture::fromBMPFile(const char *name) {
    GLuint id = loadBMP(name);
    if (id == NULL_TEXTURE) {
        return nullptr;
    } else {
        return std::unique_ptr<Texture>(new Texture(id));
    }
}

inline std::unique_ptr<Texture> Texture::fromDDSFile(const char *name) {
    GLuint id = loadDDS(name);
    if (id == NULL_TEXTURE) {
        return nullptr;
    } else {
        return std::unique_ptr<Texture>(new Texture(id));
    }
}

}
}

#endif /* end of include guard: TEXTURE_HPP_SIHZOJQQ */

