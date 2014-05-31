#include <fstream>
#include <cstring>

#include "gl/texture/loader.hpp"
#include "logging/logging.hpp"

using namespace std;

namespace gl {
namespace texture {

GLuint loadBMP(const char *filename) {
    LOG(INFO) << "Loading BMP texture: " << filename;

    ifstream file(filename, ifstream::in | ifstream::binary);
    if (!file) {
        LOG(ERROR) << "Can't open file: " << filename;
        return NULL_TEXTURE;
    }

    // read image header
    unsigned char header[54];
    file.read((char *)header, 54);
    if (!file) {
        LOG(ERROR) << "Error opening file: " << filename;
        file.close();
        return NULL_TEXTURE;
    }

    if (header[0] != 'B' && header[1] != 'M') {
        LOG(ERROR) << "Invalid BMP file.";
        file.close();
        return NULL_TEXTURE;
    }

    // parse image header
    unsigned dataPos = *(unsigned*)&(header[0x0A]);
    unsigned imageSize = *(unsigned*)&(header[0x22]);
    unsigned width = *(unsigned*)&(header[0x12]);
    unsigned height = *(unsigned*)&(header[0x16]);

    if (imageSize == 0) {
        imageSize = width * height * 3;
    }
    if (dataPos == 0) {
        dataPos = 54;
    }

    unsigned char *data = new unsigned char[imageSize];

    // read image data
    file.seekg(dataPos);
    file.read((char *)data, imageSize);
    file.close();

    // create texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // bind
    glBindTexture(GL_TEXTURE_2D, textureID);

    // send texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;

    return textureID;
}

static const unsigned FOURCC_DXT1 = 0x31545844;
static const unsigned FOURCC_DXT3 = 0x33545844;
static const unsigned FOURCC_DXT5 = 0x35545844;

GLuint loadDDS(const char *filename) {
    LOG(INFO) << "Loading DDS texture: " << filename;

    ifstream file(filename, ifstream::in | ifstream::binary);
    if (!file) {
        LOG(ERROR) << "Error opening file: " << filename;
        return NULL_TEXTURE;
    }

    char filecode[4];
    file.read(filecode, 4);
    if (!file || strncmp(filecode, "DDS ", 4) != 0) {
        LOG(ERROR) << "Invalid DDS file." << filecode;
        file.close();
        return NULL_TEXTURE;
    }

    // read header
    unsigned char header[124];
    file.read((char *)header, 124);
    if (!file) {
        LOG(ERROR) << "Invalid DDS file.";
        file.close();
        return NULL_TEXTURE;
    }

    // parse header
    unsigned height = *(unsigned *)&(header[8]);
    unsigned width = *(unsigned *)&(header[12]);
    unsigned linearSize = *(unsigned *)&(header[16]);
    unsigned mipMapCount = *(unsigned *)&(header[24]);
    unsigned fourCC = *(unsigned *)&(header[80]);

    unsigned bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    unsigned char *buffer = new unsigned char[bufsize];

    file.read((char *)buffer, bufsize);
    file.close();

    // determine file format
    GLenum format;
    switch (fourCC) {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;

    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;

    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;

    default:
        LOG(ERROR) << "Not supported compression format.";
        delete[] buffer;
        return NULL_TEXTURE;
    }

    // create texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    // mipmaps
    unsigned blockSize = fourCC == FOURCC_DXT1 ? 8 : 16;
    unsigned offset = 0;

    for (unsigned level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3)/4) * ((height + 3)/4) * blockSize;

        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width  /= 2;
        height /= 2;

        // non power-of-2 textures
        if (width < 1) {
            width = 1;
        }
        if (height < 1) {
            height = 1;
        }
    }

    delete[] buffer;

    return textureID;
}

}
}
