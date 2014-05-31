#ifndef DATA_HPP_OK7IYD0V
#define DATA_HPP_OK7IYD0V

#include "GL/glew.h"

namespace gl {

struct BufferInfo {
    GLuint bufferID = 0;
    GLint size = 0;
    GLenum type = GL_FLOAT;
};

struct ElementBufferInfo {
    GLuint bufferID = 0;
    GLsizei count = 0;
    GLenum type = GL_UNSIGNED_INT;
};

}

#endif /* end of include guard: DATA_HPP_OK7IYD0V */

