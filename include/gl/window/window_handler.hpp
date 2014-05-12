#ifndef WINDOW_HANDLER_HPP_AEQB8XAL
#define WINDOW_HANDLER_HPP_AEQB8XAL

#include "GLFW/glfw3.h"

namespace gl {

class WindowHandler {
public:
    virtual ~WindowHandler() = default;

    virtual bool init(GLFWwindow *window) = 0;

    virtual void cleanup() = 0;

    virtual void resize(int width, int height) = 0;

    virtual void handleEvents(GLFWwindow *glfwWindow) = 0;

    virtual void mouseMove(float x, float y) = 0;

    virtual void update(float delta) = 0;

    virtual void render() = 0;
};

}

#endif /* end of include guard: WINDOW_HANDLER_HPP_AEQB8XAL */

