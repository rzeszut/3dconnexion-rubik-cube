#ifndef LOADER_HPP_5ZK9K3A1
#define LOADER_HPP_5ZK9K3A1

#include <GL/glew.h>

namespace gl {
namespace shader {

const GLuint NULL_PROGRAM = 0;

GLuint loadFromSource(const char *vertexSource, const char *fragmentSource);

GLuint loadFromFiles(const char *vertexFilePath, const char *fragmentFilePath);

}
}

#endif /* end of include guard: LOADER_HPP_5ZK9K3A1 */

