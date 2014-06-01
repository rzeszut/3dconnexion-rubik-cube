#ifndef MOUSE_HPP_8NCB2OUJ
#define MOUSE_HPP_8NCB2OUJ

#include "GLFW/glfw3.h"

namespace gl {

enum class MouseButton {
    LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
    RIGHT = GLFW_MOUSE_BUTTON_RIGHT
};

enum class MouseState {
    PRESSED = GLFW_PRESS,
    RELEASED = GLFW_RELEASE
};

}

#endif /* end of include guard: MOUSE_HPP_8NCB2OUJ */

