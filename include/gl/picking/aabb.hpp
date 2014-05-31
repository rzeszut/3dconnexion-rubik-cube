#ifndef AABB_HPP_4QVYUM7R
#define AABB_HPP_4QVYUM7R

#include "glm/glm.hpp"

namespace gl {
namespace picking {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(const glm::vec3 &min, const glm::vec3 &max);
};

// ------------------ //
// inline definitions //
// ------------------ //

inline AABB::AABB(const glm::vec3 &_min, const glm::vec3 &_max)
    : min(_min), max(_max)
{}

}
}

#endif /* end of include guard: AABB_HPP_4QVYUM7R */

