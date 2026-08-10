#include "../include/timer.h"

TimePoint start_timer() {
    return std::chrono::high_resolution_clock::now();
}

double stop_timer_us(TimePoint start) {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}
