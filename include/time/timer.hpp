#pragma once

#include "common/types.hpp"

struct Timer {
    f64 duration = 0;
    f64 current_time = 0;
    bool repeat = false;

    static Timer create(f64 duration, bool repeat = false);

    void update(f64 delta_t);
    void reset();

    f64 progress();
    bool finished();
};
