#ifndef CENTERED_CAMERA_HPP_3V1E0FLZ
#define CENTERED_CAMERA_HPP_3V1E0FLZ

#include "abstract_camera.hpp"

namespace gl {

const float DEFAULT_CENTERED_TURN_SPEED = 1.f;

class CenteredCamera : public AbstractCamera {
private:
    glm::vec3 center;

    float radius = 5;

    float horAngle = 0;
    float vertAngle = 0;

public:
    CenteredCamera(glm::vec3 center, int width, int height);
    ~CenteredCamera() = default;

    void update(float delta);

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewMatrix() const;

    void setRadius(float r);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline CenteredCamera::CenteredCamera(glm::vec3 c, int width, int height) :
    AbstractCamera(width, height),
    center(c)
{
    turnSpeed = DEFAULT_CENTERED_TURN_SPEED;
}

inline void CenteredCamera::setRadius(float r) {
    radius = r;
}

}

#endif /* end of include guard: CENTERED_CAMERA_HPP_3V1E0FLZ */

