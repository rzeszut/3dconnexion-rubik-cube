#include "gl/window/window.hpp"
#include "gl/timer/fps_capper.hpp"

#include "util/preconditions.hpp"
#include "logging/logging.hpp"

using namespace std;
using namespace std::chrono;

namespace gl {

bool Window::init() {
    if (!AbstractWindow::init()) {
        return false;
    }

    CHECK_NOT_NULL(handler, "Must set handler before using the window.");
    handler->windowReference = getGLFWwindow();

    ok = handler->init();
    if (!ok) {
        AbstractWindow::cleanup();
    }

    return ok;
}

void Window::cleanup() {
    handler->cleanup();
    AbstractWindow::cleanup();
}

bool Window::handleResize() {
    if (AbstractWindow::handleResize()) {
        handler->resize(getWidth(), getHeight());
        return true;
    } else {
        return false;
    }
}

void Window::handleEvents() {
    auto window = getGLFWwindow();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)) {
        LOG(DEBUG) << "Closing the window.";
        running = false;
    } else if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
        toggleFullscreen();
    }

    handler->handleEvents();

    // mouse move
    static bool mousePositionHack = true;
    static int passedIterations = 0;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    // this hack centers mouse position at the beginning
    if (passedIterations < 10) {
        mousePositionHack = true;
        ++passedIterations;
    }
    if (mousePositionHack && x == 0.f && y == 0.f) {
        mousePositionHack = false;
        x = getWidth() >> 1;
        y = getHeight() >> 1;
    }

    handler->mouseMove(x, y);

    // mouse buttons
    for (const auto &it : mouseButtonStates) {
        handleMouseButtonChanges(it.first, x, y);
    }
}

void Window::handleMouseButtonChanges(MouseButton button, float x, float y) {
    auto buttonState = glfwGetMouseButton(getGLFWwindow(), (int) button);
    auto previousButtonState = mouseButtonStates[button];

    if (buttonState == GLFW_PRESS && previousButtonState == MouseState::RELEASED) {
        mouseButtonStates[button] = MouseState::PRESSED;
        handler->mouseButton(button, MouseState::PRESSED, x, y);
    } else if (buttonState == GLFW_RELEASE && previousButtonState == MouseState::PRESSED) {
        mouseButtonStates[button] = MouseState::RELEASED;
        handler->mouseButton(button, MouseState::RELEASED, x, y);
    }
}

void Window::update(float delta) {
    handler->update(delta);
}

void Window::render() {
    handler->render();

    glfwSwapBuffers(getGLFWwindow());
    glfwPollEvents();
}

void Window::loop() {
    FPSCapper capper(framesPerSecond);

    while (running) {
        capper.beginLoop();

        handleEvents();
        handleResize();

        update(capper.getDelta());
        render();

        capper.cap();
    }
}

}
