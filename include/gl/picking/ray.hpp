#ifndef RAY_HPP_OP59WNWH
#define RAY_HPP_OP59WNWH

#include <utility>

#include "glm/glm.hpp"

#include "util/optional.hpp"
#include "aabb.hpp"

namespace gl {
namespace picking {

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    static Ray fromScreenPosition(float x, float y,
                                  int width, int height,
                                  const glm::mat4 &view,
                                  const glm::mat4 &projection
                                 );
};

optional::Optional<float> testRayOBBIntersection(const Ray &ray,
                                                 const AABB &aabb,
                                                 const glm::mat4 &model
                                                );

}
}

#endif /* end of include guard: RAY_HPP_OP59WNWH */

