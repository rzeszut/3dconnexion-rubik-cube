#ifndef MAIN_HANDLER_HPP_J4OUT1HL
#define MAIN_HANDLER_HPP_J4OUT1HL

#include "glm/glm.hpp"

#include "gl/window/window_handler.hpp"
#include "gl/camera/centered_camera.hpp"
#include "gl/mesh/mesh.hpp"
#include "gl/shader/program.hpp"
#include "gl/texture/texture.hpp"

namespace cube {

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class MainHandler : public gl::WindowHandler {
    std::unique_ptr<gl::mesh::Mesh> cube;
    std::unique_ptr<gl::shader::Program> program;
    std::unique_ptr<gl::texture::Texture> texture;

    gl::CenteredCamera *camera;

public:
    bool init() override;

    void cleanup() override;

    void resize(int width, int height) override;

    void handleEvents() override;

    void mouseMove(float x, float y) override;

    void update(float delta) override;

    void render() override;
};

}

#endif /* end of include guard: MAIN_HANDLER_HPP_J4OUT1HL */

