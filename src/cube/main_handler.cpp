#include <GL/glew.h>
#include <list>

#include "gl/shader/shader.hpp"
#include "gl/picking/ray.hpp"
#include "logging/logging.hpp"
#include "cube/main_handler.hpp"

using namespace gl::mesh;
using namespace gl::shader;
using namespace gl::texture;

namespace cube {

bool MainHandler::init() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetInputMode(getWindowReference(), GLFW_STICKY_KEYS, GL_TRUE);

    cube.init();

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    camera = new gl::CenteredCamera(glm::vec3(0, 0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
    camera->setRadius(15.f);
    camera->setMoveSpeed(10.f);

    return true;
}

void MainHandler::cleanup() {
    delete camera;
}

void MainHandler::resize(int width, int height) {
    LOG(DEBUG) << width << ":" << height;
    camera->resize(width, height);

    this->width = width;
    this->height = height;
}

void MainHandler::handleEvents() {
    auto window = getWindowReference();

    // key press
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera->move(gl::Direction::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera->move(gl::Direction::BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->turn(gl::Turn::X_INCREASE);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->turn(gl::Turn::X_DECREASE);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->turn(gl::Turn::Y_INCREASE);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->turn(gl::Turn::Y_DECREASE);
    }

    // key release
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
        camera->stopMoving(gl::Direction::FORWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        camera->stopTurning(gl::Turn::X_INCREASE);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        camera->stopTurning(gl::Turn::Y_INCREASE);
    }
}

void MainHandler::mouseButton(gl::MouseButton button, gl::MouseState state, float x, float y) {
    if (button == gl::MouseButton::LEFT && state == gl::MouseState::RELEASED) {
        auto ray = gl::picking::Ray::fromScreenPosition(
            x, y, width, height,
            camera->getViewMatrix(), camera->getProjectionMatrix());

        auto result = cube.testRayIntersection(ray);
    }
}

void MainHandler::mouseMove(float, float) {
}

void MainHandler::update(float delta) {
    camera->update(delta);

    // 90 degrees per second
    static float halfPi = 3.1415926536 / 2.f;
    cube.rotateX(-1, delta * halfPi);
}

void MainHandler::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera matrices
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    cube.render(view, projection);
}

}

