#ifndef WINDOW_HPP_Z9O3Z851
#define WINDOW_HPP_Z9O3Z851

#include "abstract_window.hpp"
#include "window_handler.hpp"

namespace gl {

const int DEFAULT_FPS = 30;

class Window : public AbstractWindow {
private:
    WindowHandler *handler;
    bool running;
    int framesPerSecond = DEFAULT_FPS;

public:
    Window(int width, int height, const char *title);
    ~Window();

    bool init() override;
    void cleanup() override;
    bool handleResize() override;
    void handleEvents() override;
    void update(float delta) override;
    void render() override;

    void loop();

    void setHandler(WindowHandler *handler);
    void setFPS(int fps);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Window::Window(int width, int height, const char *title)
    : AbstractWindow(width, height, title),
    running(true)
{}

inline Window::~Window() {
    delete handler;
}

inline void Window::setHandler(WindowHandler *handler) {
    this->handler = handler;
}

inline void Window::setFPS(int fps) {
    framesPerSecond = fps;
}

}

#endif /* end of include guard: WINDOW_HPP_Z9O3Z851 */

