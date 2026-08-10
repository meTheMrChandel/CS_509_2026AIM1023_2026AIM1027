#ifndef TIMER_H
#define TIMER_H

#include <chrono>

typedef std::chrono::high_resolution_clock::time_point TimePoint;

// Starts the timer
TimePoint start_timer();

// Returns the elapsed time in microseconds
double stop_timer_us(TimePoint start);

#endif // TIMER_H
