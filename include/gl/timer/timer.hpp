#ifndef TIMER_HPP_LFCGJAHQ
#define TIMER_HPP_LFCGJAHQ

#include <chrono>

namespace gl {

const int TICKS_IN_SECOND = 1000;

class Timer {
private:
    std::chrono::system_clock::time_point startTime;

    bool started {false};

public:
    void start();
    void stop();

    bool isStarted();

    long getTicks();
};

// ------------------ //
// inline definitions //
// ------------------ //
inline void Timer::start() {
    using std::chrono::system_clock;

    started = true;
    startTime = system_clock::now();
}

inline void Timer::stop() {
    started = false;
}

inline bool Timer::isStarted() {
    return started;
}

inline long Timer::getTicks() {
    using namespace std::chrono;

    if (started) {
        auto ticks = duration_cast<milliseconds>(system_clock::now() - startTime);
        return ticks.count();
    } else {
        return 0;
    }
}

}

#endif /* end of include guard: TIMER_HPP_LFCGJAHQ */

