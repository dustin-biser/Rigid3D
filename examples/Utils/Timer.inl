#include "Timer.hpp"

//----------------------------------------------------------------------------------------
inline Timer::Timer()
    : counter(0),
      timerIsRunning(false) {

}

//----------------------------------------------------------------------------------------
inline void Timer::start() {
    if (!timerIsRunning) {
        timerIsRunning = true;
    }
    time1 = std::chrono::steady_clock::now();
}

//----------------------------------------------------------------------------------------
inline void Timer::stop() {
    using namespace std::chrono;

    if (timerIsRunning) {
        time2 = std::chrono::steady_clock::now();
        timerIsRunning = false;
        ++counter;
        elapsedTime = duration_cast<duration<double> >(time2 - time1);
        totalElapsedTime += elapsedTime;
    }
}

//----------------------------------------------------------------------------------------
/**
* This resets the number of iterations the timer has completed matching start() and
* stop() calls.
*/
inline void Timer::resetIterationCounter() {
    counter = 0;
}

//----------------------------------------------------------------------------------------
/**
* Returns the elapsed time in seconds for the duration pasted between the last start()
* and stop() call.
*/
inline double Timer::getElapsedTime() const {
    return elapsedTime.count();
}

//----------------------------------------------------------------------------------------
/**
* Returns the average elapsed time in seconds for the duration pasted between the all
* start() and stop() calls.
*/
inline double Timer::getAverageElapsedTime() const {
    if (counter > 0) {
        return totalElapsedTime.count() / double(counter);
    } else {
        return 0.0;
    }
}
