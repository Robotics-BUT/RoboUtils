#include <iostream>
#include <utility>
#include "roboutils/Stopwatch.h"
#include "roboutils/util/timing.h"

using namespace RoboUtils;
using namespace std;

Stopwatch::Stopwatch(std::string lbl)
 : Time(millis()), Label(std::move(lbl))
{
}

Stopwatch::~Stopwatch()
{
    Trigger(true);
}

long long Stopwatch::Trigger(bool print)
{
    auto stop = millis();
    auto diff = stop - Time;
    Time = stop;

    if (print)
        std::cout << "Measured time for label: " << Label << " is: " << diff << " ms" << std::endl;

    return diff;
}

