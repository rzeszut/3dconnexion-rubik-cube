#include <exception>

// must include glew before all other GL libraries
#include "GL/glew.h"

#include "gl/window/window.hpp"
#include "logging/logging.hpp"

#include "cube/main_handler.hpp"

int main() try {
    logging::setLevel(logging::DEBUG);
    logging::addOutput(new logging::StderrOutput);

    gl::Window window(cube::WINDOW_WIDTH, cube::WINDOW_HEIGHT, "3D Rubik cube", new cube::MainHandler);

    if (window.init()) {
        window.loop();
        window.cleanup();
    }

    return 0;
} catch (const std::exception &e) {
    LOG(ERROR) << e.what();
}

