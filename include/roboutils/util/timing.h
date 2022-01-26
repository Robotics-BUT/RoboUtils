//
// Created by Matous Hybl on 2018-10-14.
//

#pragma once

#include <chrono>
#include <unistd.h>

namespace RoboUtils {

    inline long long millis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    inline void delay(long ms) {
        usleep(ms * 1000);
    }
}
