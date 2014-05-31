#ifndef INDEX_HPP_JRSYHEAH
#define INDEX_HPP_JRSYHEAH

#include <vector>

#include "glm/glm.hpp"

namespace gl {
namespace mesh {

void indexMesh(const std::vector<glm::vec3> &verticesIn,
               const std::vector<glm::vec3> &normalsIn,
               const std::vector<glm::vec2> &uvsIn,
               std::vector<glm::vec3> &verticesOut,
               std::vector<glm::vec3> &normalsOut,
               std::vector<glm::vec2> &uvsOut,
               std::vector<unsigned> &elementsOut
              );

}
}

#endif /* end of include guard: INDEX_HPP_JRSYHEAH */

