
#pragma once

#include <chrono>

/**
 * Timer class records timing information in seconds.
 */
class Timer {

public:
    Timer();
    ~Timer() = default;

    void start();
    void stop();
    void resetIterationCounter();

    double getElapsedTime() const;
    double getAverageElapsedTime() const;


private:
    std::chrono::steady_clock::time_point time1;
    std::chrono::steady_clock::time_point time2;
    std::chrono::duration<double> elapsedTime;
    std::chrono::duration<double> totalElapsedTime;

    unsigned int counter;
    bool timerIsRunning;
};

#include "Timer.inl"