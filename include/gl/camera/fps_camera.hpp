#ifndef FPS_CAMERA_HPP_9JFITWBC
#define FPS_CAMERA_HPP_9JFITWBC

#include "GLFW/glfw3.h"

#include "abstract_camera.hpp"

namespace gl {

class FPSCamera : public AbstractCamera {
private:
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;

    float xPos;
    float yPos;

    GLFWwindow *window;

public:
    FPSCamera(GLFWwindow *window, unsigned width, unsigned height);
    ~FPSCamera() = default;

    void update(float delta) override;

    glm::mat4 getProjectionMatrix() const override;
    glm::mat4 getViewMatrix() const override;

    void lookAround(float x, float y);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline FPSCamera::FPSCamera(GLFWwindow *win, unsigned width, unsigned height) :
    AbstractCamera(width, height),
    window(win)
{
    // TODO: refactor this
    position = glm::vec3{0, 0, 5};
}

inline void FPSCamera::lookAround(float x, float y) {
    xPos = x;
    yPos = y;
}

}

#endif /* end of include guard: FPS_CAMERA_HPP_9JFITWBC */

