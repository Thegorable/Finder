#include "Time.h"

using namespace std;
using namespace chrono;

Timer::Timer(size_t sec) : seconds_duration(sec), is_expired(false) {
    SetTimerDuration(seconds_duration);
    start_time_point = steady_clock::now();
}

void Timer::SetTimerDuration(size_t sec) {
    timer_duration = duration_cast<nanoseconds>(seconds(sec));
}

void Timer::UpdateTimer() {
    timer_duration -= (steady_clock::now() - start_time_point);
    start_time_point = steady_clock::now();

    if (timer_duration.count() <= 0) {
        is_expired = true;
    }
}

bool Timer::IsExpired() {
    return is_expired;
}

void Timer::RestartTimer() {
    is_expired = false;
    start_time_point = steady_clock::now();
    SetTimerDuration(seconds_duration);
}

std::ostream& operator << (std::ostream& os, std::chrono::nanoseconds n) {
    os << n.count() << " nanoseconds";
    return os;
}

std::ostream& operator << (std::ostream& os, std::chrono::microseconds n) {
    os << n.count() << " microseconds";
    return os;
}

std::ostream& operator << (std::ostream& os, std::chrono::milliseconds n) {
    os << n.count() << " milliseconds";
    return os;
}

std::ostream& operator << (std::ostream& os, std::chrono::seconds n) {
    os << n.count() << " seconds";
    return os;
}

void StopWatch::Restart() {
    stopwatch_dur = 0ns;
    start_time_point = steady_clock::now();
}

void StopWatch::Pause() {
    stopwatch_dur += GetStopWatch();
}

void StopWatch::Continue() {
    start_time_point = steady_clock::now();
}

nanoseconds StopWatch::GetStopWatch() {
    return steady_clock::now() - start_time_point;
}

MaxFPS::MaxFPS(size_t max_fps) : StopWatch::StopWatch() {
    max_fps_ = duration_cast<nanoseconds>(seconds(1s)) / max_fps;
}

void MaxFPS::StartFunction() {
    Restart();
}

void MaxFPS::EndFunction() {
    nanoseconds delta = max_fps_ - GetStopWatch();
    this_thread::sleep_for(delta);
}
