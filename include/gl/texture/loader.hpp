#ifndef LOADER_HPP_E2FSBZWJ
#define LOADER_HPP_E2FSBZWJ

#include "GL/glew.h"

namespace gl {
namespace texture {

constexpr GLuint NULL_TEXTURE = 0;

GLuint loadBMP(const char *filename);
GLuint loadDDS(const char *filename);

}
}

#endif /* end of include guard: LOADER_HPP_E2FSBZWJ */

