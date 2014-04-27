#ifndef MODEL_HPP_EU8Z3YGP
#define MODEL_HPP_EU8Z3YGP

#include <vector>

#include "glm/glm.hpp"

namespace gl {

struct Model {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
};

}

#endif /* end of include guard: MODEL_HPP_EU8Z3YGP */

