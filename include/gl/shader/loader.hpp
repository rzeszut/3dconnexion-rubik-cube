#ifndef LOADER_HPP_OUGEAVRJ
#define LOADER_HPP_OUGEAVRJ

#include <vector>

#include "GL/glew.h"

namespace gl {
namespace shader {

const GLuint NULL_SHADER = 0;
const GLuint NULL_PROGRAM = 0;

GLuint loadShaderFromFile(const char *filename, GLenum type);
GLuint loadShaderFromSource(const char *source, GLenum type);

GLuint compileProgram(const std::vector<GLuint> &shaders);

}
}

#endif /* end of include guard: LOADER_HPP_OUGEAVRJ */

