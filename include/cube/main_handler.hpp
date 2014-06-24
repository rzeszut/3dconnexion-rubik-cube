#ifndef MAIN_HANDLER_HPP_J4OUT1HL
#define MAIN_HANDLER_HPP_J4OUT1HL

#include "glm/glm.hpp"

#include "gl/window/window_handler.hpp"
#include "gl/camera/centered_camera.hpp"

namespace cube {

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class MainHandler : public gl::WindowHandler {
    gl::CenteredCamera *camera;

    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;

public:
    bool init() override;

    void cleanup() override;

    void resize(int width, int height) override;

    void handleEvents() override;

    void mouseMove(float x, float y) override;

    void mouseButton(gl::MouseButton button, gl::MouseState state, float x, float y);

    void update(float delta) override;

    void render() override;
};

}

#endif /* end of include guard: MAIN_HANDLER_HPP_J4OUT1HL */

