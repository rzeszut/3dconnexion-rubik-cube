#ifndef ABSTRACT_WINDOW_HPP_VEG4P8O1
#define ABSTRACT_WINDOW_HPP_VEG4P8O1

#include "GLFW/glfw3.h"

namespace gl {

/**
 * Class representing a window.
 */
class AbstractWindow {
private:
    GLFWwindow *winReference;
    int width;
    int height;
    const char *title;

    void resize(int, int);
    void changeFullscreen();

protected:
    /**
     * Is the window ok.
     */
    bool ok;

    /**
     * Fullscreens yes/no. Defaults to false.
     */
    bool fullscreen;

    /**
     * Returns the pointer wo the GLFWwindow structure.
     * \return window GLFW window reference.
     */
    GLFWwindow *getGLFWwindow() const;

public:
    /**
     * Constructs the window.
     * \param width Width of the window.
     * \param height Height of the window.
     */
    AbstractWindow(int width, int height, const char *title);

    /**
     * Destructor.
     */
    virtual ~AbstractWindow() {}

    /**
     * Initializes the window and OpenGL context. On error, return false and sets 'ok' property to false.
     * \returns True if no error happened.
     */
    virtual bool init();

    /**
     * Cleans up the window.
     */
    virtual void cleanup();

    /**
     * Detects changes in window size and resizes the viewport adequatelly.
     * \return true if window size changed, false otherwise
     */
    virtual bool handleResize();

    /**
     * Handles events. On default, the only event handled is pressing the key <Esc> - closing the window.
     */
    virtual void handleEvents() = 0;

    /**
     * Updates the state. To be overriden.
     */
    virtual void update(float delta) = 0;

    /**
     * Renders the contents of the window. To be overriden.
     */
    virtual void render() = 0;

    /**
     * Toggles fulscreen mode.
     */
    void toggleFullscreen();

    /**
     * Returns 'ok'.
     * \return True if no error happened, false otherwise.
     */
    bool isOk() const;

    /**
     * Sets the title of the window.
     * \param title A new title.
     */
    void setTitle(const char *title);

    /**
     * Sets whether app is fullscreen.
     * \param fullscreen Yes/no.
     */
    void setFullscreen(bool fullscreen);

    /**
     * Return width of the window.
     * \return width
     */
    int getWidth() const;

    /**
     * Return height of the window.
     * \return height
     */
    int getHeight() const;
};

// ------------------ //
// inline definitions //
// ------------------ //
inline bool AbstractWindow::isOk() const {
    return ok;
}

inline void AbstractWindow::toggleFullscreen() {
    fullscreen = !fullscreen;
    changeFullscreen();
}

inline void AbstractWindow::setTitle(const char *title) {
    this->title = title;
    if (winReference) {
        glfwSetWindowTitle(winReference, title);
    }
}

inline void AbstractWindow::setFullscreen(bool f) {
    fullscreen = f;
    changeFullscreen();
}

inline int AbstractWindow::getWidth() const {
    return width;
}

inline int AbstractWindow::getHeight() const {
    return height;
}

inline GLFWwindow *AbstractWindow::getGLFWwindow() const {
    return winReference;
}

}

#endif /* end of include guard: ABSTRACT_WINDOW_HPP_VEG4P8O1 */

