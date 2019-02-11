//
// Created by Matous Hybl on 2018-11-14.
//

#ifndef FIRMWARE_THREADING_H
#define FIRMWARE_THREADING_H

#include <functional>

namespace RoboUtils {
    void repeatAsynchronously(unsigned int periodMs, const std::function<void(void)> &fun);
};

#endif //FIRMWARE_THREADING_H
