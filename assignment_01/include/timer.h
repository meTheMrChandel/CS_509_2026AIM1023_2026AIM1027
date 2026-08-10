#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// High-resolution clock aliases.
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

// Start high-resolution clock timer.
TimePoint start_timer();

// Stop timer and compute elapsed milliseconds since t_start.
double stop_timer(TimePoint t_start);

#endif // TIMER_H
