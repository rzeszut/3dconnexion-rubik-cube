#ifndef SHADER_HPP_QTAMZJ60
#define SHADER_HPP_QTAMZJ60

#include <GL/glew.h>

namespace gl {
namespace shader {

const GLuint NULL_PROGRAM = 0;

GLuint loadFromSource(const char *vertexSource, const char *fragmentSource);

GLuint loadFromFiles(const char *vertexFilePath, const char *fragmentFilePath);

}
}

#endif /* end of include guard: SHADER_HPP_QTAMZJ60 */

