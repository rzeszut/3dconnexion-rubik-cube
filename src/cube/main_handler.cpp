#include <GL/glew.h>

#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "logging/logging.hpp"

#include "cube/main_handler.hpp"

namespace cube {

bool MainHandler::onInit(GLFWwindow *window) {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // load model
    cube = gl::ObjModelFactory::getInstance().fromFile("resources/cube.obj");
    if (!cube) {
        return false;
    }

    // load shaders
    programID = gl::shader::loadFromFiles("resources/vertex.vert", "resources/fragment.frag");
    if (programID == gl::shader::NULL_PROGRAM) {
        return false;
    }

    // load texture
    textureID = gl::texture::loadDDS("resources/uvmap.dds");
    if (textureID == gl::texture::NULL_TEXTURE) {
        glDeleteProgram(programID);
        return false;
    }
    textureUniform = glGetUniformLocation(programID, "textureSampler");

    // mvp matrix
    matrixID = glGetUniformLocation(programID, "MVP");

    // vertex buffer creation
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube->vertices.size(), &cube->vertices[0], GL_STATIC_DRAW);

    // uv buffer creation
    glGenBuffers(1, &uvBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * cube->uvs.size(), &cube->uvs[0], GL_STATIC_DRAW);

    // normal buffer creation
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube->normals.size(), &cube->normals[0], GL_STATIC_DRAW);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    camera = new gl::CenteredCamera(glm::vec3(0, 0, 0), WINDOW_WIDTH, WINDOW_HEIGHT);

    return true;
}

void MainHandler::onCleanup() {
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &uvBufferID);
    glDeleteBuffers(1, &normalBufferID);
    glDeleteProgram(programID);
    glDeleteTextures(1, &textureID);

    delete camera;
}

void MainHandler::onResize(int width, int height) {
    LOG(DEBUG) << width << ":" << height;
    camera->resize(width, height);
}

void MainHandler::onHandleEvents(GLFWwindow *window) {
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
        camera->turn(gl::Turn::Y_DECREASE);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->turn(gl::Turn::Y_INCREASE);
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

void MainHandler::onMouseMove(float, float) {
}

void MainHandler::onUpdate(float delta) {
    camera->update(delta);

    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model(1.f);
    mvp = projection * view * model;
}

void MainHandler::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use shaders
    glUseProgram(programID);

    // send MVP matrix
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

    // bind texture to unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureUniform, 0);

    // send vertex buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // send uv buffer
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // send normal buffer
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

}

