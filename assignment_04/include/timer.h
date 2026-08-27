#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

TimePoint start_timer();
double stop_timer(TimePoint t_start);

#endif // TIMER_H
