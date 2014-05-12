#include <GL/glew.h>

#include "gl/texture.hpp"
#include "logging/logging.hpp"

#include "cube/main_handler.hpp"

namespace cube {

bool MainHandler::init(GLFWwindow *window) {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // load model
    auto cubeModel = gl::model::ObjModelLoader::fromFile("resources/cube.obj");
    if (!cubeModel) {
        return false;
    }
    cube = gl::model::IndexedModel::fromModel(std::move(cubeModel));
    if (!cube) {
        return false;
    }
    cube->init();

    // load shaders
    program = gl::shader::Program::fromFiles("resources/vertex.vert", "resources/fragment.frag");
    if (!program) {
        return false;
    }

    // load texture
    textureID = gl::texture::loadDDS("resources/uvmap.dds");
    if (textureID == gl::texture::NULL_TEXTURE) {
        cube->cleanup();
        return false;
    }

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    camera = new gl::CenteredCamera(glm::vec3(0, 0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);

    return true;
}

void MainHandler::cleanup() {
    glDeleteTextures(1, &textureID);
    cube->cleanup();

    delete camera;
}

void MainHandler::resize(int width, int height) {
    LOG(DEBUG) << width << ":" << height;
    camera->resize(width, height);
}

void MainHandler::handleEvents(GLFWwindow *window) {
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

void MainHandler::mouseMove(float, float) {
}

void MainHandler::update(float delta) {
    camera->update(delta);
}

void MainHandler::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model(1.f);
    glm::mat4 modelViewProjection = projection * view * model;

    WITH_PROGRAM(*program) {
        program->setUniform("MVP", modelViewProjection);
        program->setTexture2D("textureSampler", 0, textureID);

        program->setAttribute("vertexPosition", cube->getVertexBufferInfo());
        program->setAttribute("vertexUV", cube->getUVBufferInfo());

        program->drawElements(GL_TRIANGLES, cube->getElementBufferInfo());
    }
}

}

