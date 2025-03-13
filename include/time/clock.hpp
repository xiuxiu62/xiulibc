#pragma once

#include "common/types.hpp"

#include <chrono>

struct Clock {
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point previous_time;
    f64 elapsed = 0;
    f64 delta_t = 0;
    f32 time_scale = 1;
    usize days = 0;

    static Clock create();
    void update();
};
