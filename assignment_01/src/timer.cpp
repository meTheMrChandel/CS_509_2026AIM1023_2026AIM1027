#include "../include/timer.h"

// Start timing using high-resolution clock now.
TimePoint start_timer()
{
    return Clock::now();
}

// Stop timing and compute duration count in milliseconds.
double stop_timer(TimePoint t_start)
{
    TimePoint t_end = Clock::now();
    return std::chrono::duration<double, std::milli>(t_end - t_start).count();
}
