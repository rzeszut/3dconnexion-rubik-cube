#ifndef TEXTURE_HPP_IAHKTFYN
#define TEXTURE_HPP_IAHKTFYN

#include <GL/glew.h>

namespace gl {
namespace texture {

const GLuint NULL_TEXTURE = 0;

GLuint loadBMP(const char *filename);

GLuint loadDDS(const char *filename);

}
}

#endif /* end of include guard: TEXTURE_HPP_IAHKTFYN */

