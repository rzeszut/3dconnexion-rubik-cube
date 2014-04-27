#ifndef WINDOW_HANDLER_HPP_AEQB8XAL
#define WINDOW_HANDLER_HPP_AEQB8XAL

#include "GLFW/glfw3.h"

namespace gl {

class WindowHandler {
public:
    virtual ~WindowHandler() = default;

    virtual bool onInit(GLFWwindow *window) = 0;

    virtual void onCleanup() = 0;

    virtual void onResize(int width, int height) = 0;

    virtual void onHandleEvents(GLFWwindow *glfwWindow) = 0;

    virtual void onMouseMove(float x, float y) = 0;

    virtual void onUpdate(float delta) = 0;

    virtual void onRender() = 0;
};

}

#endif /* end of include guard: WINDOW_HANDLER_HPP_AEQB8XAL */

