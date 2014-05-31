#ifndef OBJ_LOADER_HPP_LOZA9WJM
#define OBJ_LOADER_HPP_LOZA9WJM

#include <vector>

#include "glm/glm.hpp"

namespace gl {
namespace mesh {

bool loadOBJ(const char *filename,
             std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &normals,
             std::vector<glm::vec2> &uvs
            );

}
}

#endif /* end of include guard: OBJ_LOADER_HPP_LOZA9WJM */

