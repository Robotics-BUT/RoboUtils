//
// Created by Matous Hybl on 2018-11-14.
//

#include "roboutils/util/timing.h"

#include <chrono>
#include <unistd.h>

long long RoboUtils::millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

void RoboUtils::delay(long ms)
{
    usleep(ms * 1000);
}

bool RoboUtils::expired(long long *last_event, long delay, long long ms)
{
    if (!last_event)
        return false;

    if (ms < 0)
        ms = millis();

    if (ms < (*last_event + delay))
        return false;

    *last_event = ms;
    return true;
}