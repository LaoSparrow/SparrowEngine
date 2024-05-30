#include "sparrow_engine/time.hpp"

using namespace SparrowEngine;

double Time::last_time;
double Time::now;
double Time::delta_time;

double Time::GetDeltaTime() {
    return delta_time;
}

double Time::GetTimeNow() {
    return now;
}