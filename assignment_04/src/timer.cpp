#include "../include/timer.h"

TimePoint start_timer()
{
    return Clock::now();
}

double stop_timer(TimePoint t_start)
{
    TimePoint t_end = Clock::now();
    return std::chrono::duration<double, std::milli>(t_end - t_start).count();
}
