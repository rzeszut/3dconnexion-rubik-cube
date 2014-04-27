#ifndef MAIN_HANDLER_HPP_J4OUT1HL
#define MAIN_HANDLER_HPP_J4OUT1HL

#include "glm/glm.hpp"

#include "gl/window/window_handler.hpp"
#include "gl/camera/centered_camera.hpp"
#include "gl/model/obj_model_factory.hpp"

namespace cube {

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class MainHandler : public gl::WindowHandler {
    std::shared_ptr<gl::Model> cube;
    GLuint vertexBufferID;
    GLuint uvBufferID;
    GLuint normalBufferID;

    GLuint textureID;
    GLuint textureUniform;

    GLuint programID;

    glm::mat4 mvp;
    GLuint matrixID;

    gl::CenteredCamera *camera;

public:
    bool onInit(GLFWwindow *window) override;

    void onCleanup() override;

    void onResize(int width, int height) override;

    void onHandleEvents(GLFWwindow *window) override;

    void onMouseMove(float x, float y) override;

    void onUpdate(float delta) override;

    void onRender() override;
};

}

#endif /* end of include guard: MAIN_HANDLER_HPP_J4OUT1HL */

