#include "util/preconditions.hpp"
#include "gl/texture/texture.hpp"

namespace gl {
namespace texture {

static GLint getGLTextureWrapType(TextureWrap wrap) {
    switch (wrap) {
    case TextureWrap::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;

    case TextureWrap::REPEAT:
        return GL_REPEAT;

    case TextureWrap::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;

    default:
        return 0;
    }
}

static GLint getGLTextureFilterType(TextureFilter filter) {
    switch (filter) {
    case TextureFilter::NEAREST:
        return GL_NEAREST;

    case TextureFilter::LINEAR:
        return GL_LINEAR;

    case TextureFilter::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;

    case TextureFilter::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;

    case TextureFilter::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;

    case TextureFilter::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;

    default:
        return 0;
    }
}

void Texture::setMinificationFilter(TextureFilter minFilter) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLTextureFilterType(minFilter));
}

void Texture::setMagnificationFilter(TextureFilter magFilter) {
    bool valid = magFilter == TextureFilter::NEAREST || magFilter == TextureFilter::LINEAR;
    CHECK(valid, "Magnification filter must Be GL_LINEAR or GL_NEAREST");

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLTextureFilterType(magFilter));
}

void Texture::setWrap(TextureWrap wrapS, TextureWrap wrapT) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLTextureWrapType(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLTextureWrapType(wrapT));
}

}
}

