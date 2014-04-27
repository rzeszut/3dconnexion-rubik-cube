#include <cmath>

#include "glm/gtc/matrix_transform.hpp"

#include "gl/camera/fps_camera.hpp"

using namespace glm;

namespace gl {

void FPSCamera::update(float delta) {
    // TODO: rewrite this method to be similar to CenteredCamera
    horizontalAngle += (xOrigin - xPos) * turnSpeed * delta;
    verticalAngle += (yOrigin - yPos) * turnSpeed * delta;

    glfwSetCursorPos(window, xOrigin, yOrigin);

    direction = vec3(cos(verticalAngle) * sin(horizontalAngle),
                     sin(verticalAngle),
                     cos(verticalAngle) * cos(horizontalAngle));

    vec3 right = vec3(sin(horizontalAngle - HALF_PI),
                 0,
                 cos(horizontalAngle - HALF_PI));

    up = cross(right, direction);

    position += direction * delta * moveSpeed * (float) forwardBackward;
    position += right * delta * moveSpeed * (float) rightLeft;
}

glm::mat4 FPSCamera::getProjectionMatrix() const {
    return perspective(60.f, xOrigin / (float) yOrigin, 0.1f, 100.f);
}

glm::mat4 FPSCamera::getViewMatrix() const {
    return lookAt(position,
                  position + direction,
                  up);
}

}

