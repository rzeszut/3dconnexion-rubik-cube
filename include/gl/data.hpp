#ifndef DATA_HPP_OK7IYD0V
#define DATA_HPP_OK7IYD0V

#include "GL/glew.h"

namespace gl {

struct BufferInfo {
    GLuint bufferID;
    GLint size;
    GLenum type;
};

struct ElementsInfo {
    GLuint bufferID;
    GLsizei count;
    GLenum type;
};

}

#endif /* end of include guard: DATA_HPP_OK7IYD0V */

