//
// Created by Matous Hybl on 2018-10-14.
//

#ifndef FIRMWARE_UTILS_H
#define FIRMWARE_UTILS_H

#include <chrono>
#include <unistd.h>

namespace RoboUtils {
#define var auto
#define let const auto
#define guard(condition) if (condition) {}

    inline long long millis() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    inline void delay(long ms) {
        usleep(ms * 1000);
    }
};

#endif //FIRMWARE_UTILS_H
