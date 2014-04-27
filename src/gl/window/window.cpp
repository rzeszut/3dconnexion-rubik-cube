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
    ok = handler->onInit(getGLFWwindow());
    if (!ok) {
        AbstractWindow::cleanup();
    }

    return ok;
}

void Window::cleanup() {
    handler->onCleanup();
    AbstractWindow::cleanup();
}

bool Window::handleResize() {
    if (AbstractWindow::handleResize()) {
        handler->onResize(getWidth(), getHeight());
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

    handler->onHandleEvents(window);

    // mouse
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

    handler->onMouseMove(x, y);
}

void Window::update(float delta) {
    handler->onUpdate(delta);
}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    handler->onRender();

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
