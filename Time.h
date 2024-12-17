#pragma once
#include <thread>
#include <iostream>

class Timer{
public:
    Timer() = default;
    Timer(std::size_t sec);

    void SetTimerDuration(std::size_t sec);
    void UpdateTimer();
    bool IsExpired();
    void RestartTimer();

protected:
    std::size_t seconds_duration = 0;
    std::chrono::nanoseconds timer_duration;
    bool is_expired = false;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start_time_point;
};

class StopWatch {
public:
    StopWatch() = default;

    void Restart();
    void Pause();
    void Continue();

protected:
    std::chrono::nanoseconds GetStopWatch();
    
    std::chrono::nanoseconds stopwatch_dur{};
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start_time_point = std::chrono::steady_clock::now();
};

std::ostream& operator << (std::ostream& os, std::chrono::nanoseconds);
std::ostream& operator << (std::ostream& os, std::chrono::microseconds);
std::ostream& operator << (std::ostream& os, std::chrono::milliseconds);
std::ostream& operator << (std::ostream& os, std::chrono::seconds);

class Profiler : public StopWatch {
public:
    template <class TimeType>
    inline void PrintResults();
    template <class TimeType>
    inline void PrintResults(std::string m);
};

template<class TimeType>
inline void Profiler::PrintResults() {
    stopwatch_dur += GetStopWatch();
    start_time_point = std::chrono::steady_clock::now();
    std::cerr << duration_cast<TimeType>(std::chrono::nanoseconds(stopwatch_dur)) << " " << '\n';
    Restart();
}

template<class TimeType>
inline void Profiler::PrintResults(std::string m) {
    std::cerr << m;
    PrintResults<TimeType>();
}


class MaxFPS : StopWatch {
public:
    MaxFPS() = default;
    MaxFPS(std::size_t max_fps);
    
    void StartFunction();
    void EndFunction();

private:
    std::chrono::nanoseconds max_fps_;
};