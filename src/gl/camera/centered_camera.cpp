#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "gl/camera/centered_camera.hpp"
#include "logging/logging.hpp"

using namespace glm;

namespace gl {

void CenteredCamera::update(float delta) {
    horAngle += delta * turnSpeed * (float) turnY;
    vertAngle += delta * turnSpeed * (float) turnX;

    radius -= delta * moveSpeed * (float) forwardBackward;

    direction = vec3(cos(vertAngle) * sin(horAngle),
                      sin(vertAngle),
                      cos(vertAngle) * cos(horAngle));

    vec3 right = vec3(sin(horAngle + HALF_PI),
                      0,
                      cos(horAngle + HALF_PI));

    up = cross(right, -direction);

    position = center + radius * direction;
}

glm::mat4 CenteredCamera::getProjectionMatrix() const {
    return perspective(60.f, xOrigin / (float) yOrigin, 0.1f, 100.f);
}

glm::mat4 CenteredCamera::getViewMatrix() const {
    return lookAt(position,
                  center,
                  up);
}

}

