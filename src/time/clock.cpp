#include "time/clock.hpp"

#define MAX_ELAPSED_TIME (24.0 * 60.0 * 60.0) // 24 hours in seconds

Clock Clock::create() {
    Clock clock;
    auto now = std::chrono::high_resolution_clock::now();
    clock.start_time = now;
    clock.previous_time = now;
    return clock;
}

void Clock::update() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<f64>(now - previous_time);
    delta_t = duration.count() * time_scale;
    elapsed += delta_t;
    previous_time = now;

    if (elapsed >= MAX_ELAPSED_TIME) {
        elapsed -= MAX_ELAPSED_TIME;
        days++;
    }
}
