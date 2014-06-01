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

    // rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        rotationDirection = -1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        rotationDirection = 1;
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

    //rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
        rotationDirection = 0;
    }
}

// FIXME: messy code in the next two methods
void MainHandler::mouseButton(gl::MouseButton button, gl::MouseState state, float x, float y) {
    if (state != gl::MouseState::RELEASED) {
        return;
    }

    if (button == gl::MouseButton::LEFT) {
        auto ray = gl::picking::Ray::fromScreenPosition(
            x, y, width, height,
            camera->getViewMatrix(), camera->getProjectionMatrix());

        auto result = cube.testRayIntersection(ray);
        if (result) {
            // save clicked cube
            if (!coords1) {
                coords1 = result;
            } else if (!coords2) {
                coords2 = result;
            } else if (!coords3) {
                coords3 = result;
                LOG(DEBUG) << "All coords present.";
            }
            // if none of coords is vacant, ignore
        }
    } else if (button == gl::MouseButton::RIGHT) {
        // right button resets the selection
        LOG(DEBUG) << "Resetting coords.";
        coords1.reset();
        coords2.reset();
        coords3.reset();
        // turn off rotation
        rotationEnabled = false;
        rotationDirection = 0;
    }
}

void MainHandler::mouseMove(float, float) {
}

void MainHandler::update(float delta) {
    camera->update(delta);

    // update application state
    bool allCoordsPresent = coords1 && coords2 && coords3;
    if (allCoordsPresent && !rotationEnabled) {
        auto result = findCommonAxis(coords1.get(), coords2.get(), coords3.get());
        rotationEnabled = result;
        if (result) {
            LOG(DEBUG) << "Found common axis.";
            rotation = result.get();
        } else {
            rotationDirection = 0;
        }
    }

    // 90 degrees per second
    static float halfPi = 3.1415926536 / 2.f;
    if (rotationEnabled) {
        cube.rotate(rotation.first, rotation.second, rotationDirection * delta * halfPi);
    }
}

void MainHandler::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera matrices
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    cube.render(view, projection);
}

}

