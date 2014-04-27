#include <thread>
#include <chrono>

#include <GL/glew.h>
#include "gl/window/abstract_window.hpp"
#include "logging/logging.hpp"

using namespace std;

namespace gl {

AbstractWindow::AbstractWindow(int w, int h, const char *t) :
    winReference(nullptr),
    width(w),
    height(h),
    title(t),
    ok(true),
    fullscreen(false)
{}

bool AbstractWindow::init() {
    if (!glfwInit()) {
        LOG(ERROR) << "Failed to initialize gflw.";
        return ok = false;
    }

    winReference = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!winReference) {
        LOG(ERROR) << "Failed to open GLFW window.";
        glfwTerminate();
        return ok = false;
    }
    glfwMakeContextCurrent(winReference);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Failed to initialize GLEW";
        return ok = false;
    }

    resize(width, height);

    return true;
}

void AbstractWindow::cleanup() {
    LOG(DEBUG) << "AbstractWindow destroyed.";
    glfwDestroyWindow(winReference);
    glfwTerminate();
}

void AbstractWindow::changeFullscreen() {
    // TODO
//    if (fullscreen) {
//        auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
//
//        glfwCloseWindow();
//        glfwOpenWindow(videoMode->width, videoMode->height, 0, 0, 0, 0, 32, 0, GLFW_FULLSCREEN);
//        //resize(desktopMode.Width, desktopMode.Height);
//    } else {
//        glfwCloseWindow();
//        glfwOpenWindow(width, height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW);
//        //resize(width, height);
//    }
}

void AbstractWindow::resize(int w, int h) {
    if (h == 0) {
        h = 1;
    }
    width = w;
    height = h;

    glViewport(0, 0, width, height);
}

bool AbstractWindow::handleResize() {
    int w, h;
    glfwGetWindowSize(winReference, &w, &h);

    if (w == width && h == height) {
        return false;
    } else {
        resize(w, h);
        return true;
    }
}

}

