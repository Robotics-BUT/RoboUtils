//
// Created by Matous Hybl on 2018-11-14.
//

#pragma once

#include <functional>

namespace RoboUtils {

    void repeatAsynchronously(unsigned int periodMs, const std::function<bool(void)> &fun);

}

