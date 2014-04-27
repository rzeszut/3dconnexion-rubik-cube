#ifndef FPS_CAPPER_HPP_0QMBKS16
#define FPS_CAPPER_HPP_0QMBKS16

#include <thread>
#include <chrono>

#include "timer.hpp"

namespace gl {

class FPSCapper {
private:
    Timer timer;
    long currentTime = 0;
    long lastTime = 0;

    int ticksPerFrame;

public:
    explicit FPSCapper(int fps);

    void beginLoop();
    void cap();

    double getDelta() const;
};

// ------------------ //
// inline definitions //
// ------------------ //
inline FPSCapper::FPSCapper(int fps) {
    ticksPerFrame = TICKS_IN_SECOND / fps;
    timer.start();
}

inline void FPSCapper::beginLoop() {
    lastTime = currentTime;
    currentTime = timer.getTicks();
}

inline void FPSCapper::cap() {
    long ticks = timer.getTicks() - currentTime;
    if (ticks < ticksPerFrame) {
        auto waitTime = std::chrono::milliseconds(ticksPerFrame - ticks);
        std::this_thread::sleep_for(waitTime);
    }
}

inline double FPSCapper::getDelta() const {
    return (currentTime - lastTime) / (float) TICKS_IN_SECOND;
}

}

#endif /* end of include guard: FPS_CAPPER_HPP_0QMBKS16 */

