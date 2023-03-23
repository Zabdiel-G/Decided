//file name: sarboleda.h
#include <chrono>
#pragma once
using namespace std::chrono;

class Timer {
public:
    Timer();
    void reset();
    float deltaTime();
    float elapsedTime();
    system_clock::time_point _start_time, _current_time, _last_delta_tick;
    duration<float> _delta_time;
};
