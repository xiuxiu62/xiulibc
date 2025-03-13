#include "time/timer.hpp"

#include <cmath>

// TODO: accumulate proper offset deltas for resets

Timer Timer::create(f64 duration, bool repeat) {
    Timer timer;
    timer.duration = duration;
    timer.repeat = repeat;
    return timer;
}

void Timer::update(f64 delta_t) {
    current_time += delta_t;
    if (repeat && finished()) reset();
}

bool Timer::finished() {
    return current_time >= duration;
}

void Timer::reset() {
    current_time = 0.0;
}

// completion percentage of a timer (0 to 1).
f64 Timer::progress() {
    if (duration <= 0.0) return 0.0;
    return std::fmin(current_time / duration, 1.0);
}
