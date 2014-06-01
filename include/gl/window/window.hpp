#ifndef WINDOW_HPP_Z9O3Z851
#define WINDOW_HPP_Z9O3Z851

#include <map>

#include "abstract_window.hpp"
#include "window_handler.hpp"
#include "mouse.hpp"

namespace gl {

const int DEFAULT_FPS = 30;

class Window : public AbstractWindow {
private:
    WindowHandler *handler;
    bool running;
    int framesPerSecond = DEFAULT_FPS;

    std::map<MouseButton, MouseState> mouseButtonStates {
        {MouseButton::LEFT, MouseState::RELEASED},
        {MouseButton::MIDDLE, MouseState::RELEASED},
        {MouseButton::RIGHT, MouseState::RELEASED}
    };

    void handleMouseButtonChanges(MouseButton button, float x, float y);

public:
    Window(int width, int height, const char *title, WindowHandler *handler);
    ~Window();

    bool init() override;
    void cleanup() override;
    bool handleResize() override;
    void handleEvents() override;
    void update(float delta) override;
    void render() override;

    void loop();

    void setFPS(int fps);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline Window::Window(int width, int height, const char *title, WindowHandler *h)
    : AbstractWindow(width, height, title),
    handler(h),
    running(true)
{}

inline Window::~Window() {
    delete handler;
}

inline void Window::setFPS(int fps) {
    framesPerSecond = fps;
}

}

#endif /* end of include guard: WINDOW_HPP_Z9O3Z851 */

