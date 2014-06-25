#include <list>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

#include "logging/logging.hpp"
#include "cube/main_handler.hpp"

namespace cube {

using namespace glm;
using namespace optional;

bool MainHandler::init() {
    glfwSetInputMode(getWindowReference(), GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    camera = new gl::CenteredCamera(glm::vec3(0, 0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);
    camera->setRadius(15.f);
    camera->setMoveSpeed(10.f);

    cube.init();

    return true;
}

void MainHandler::cleanup() {
    delete camera;
}

void MainHandler::resize(int width, int height) {
    LOG(DEBUG) << width << ":" << height;
    camera->resize(width, height);

    viewport.z = width;
    viewport.w = height;
}

void MainHandler::handleEvents() {
    auto window = getWindowReference();

    // key press
    // camera
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
    // camera
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
    if (button != gl::MouseButton::RIGHT || cube.isRotating()) {
        return;
    }

    switch (state) {
    case gl::MouseState::PRESSED:
        mouseHoldBegin = Optional<vec2>(x, y);
        break;

    case gl::MouseState::RELEASED:
        mouseHoldEnd = Optional<vec2>(x, y);
        break;
    }
}

void MainHandler::mouseMove(float, float) {
}

void MainHandler::update(float delta) {
    camera->update(delta);

    // camera matrices
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));

    glm::vec2 wndSize = glm::vec2(viewport.z, viewport.w);
    if (mouseHoldBegin && mouseHoldEnd) {
        // initiate rotation
        cube.rotate(projection, view, viewport, wndSize, *mouseHoldEnd, *mouseHoldBegin);

        mouseHoldBegin.reset();
        mouseHoldEnd.reset();
    }

    cube.update();
}

void MainHandler::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube.draw();
}

}

