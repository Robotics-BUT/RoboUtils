//
// Created by Matous Hybl on 02/10/2018.
//
#pragma once

#include <cstdint>

namespace RoboUtils {

    template<typename T>
    static inline constexpr T saturate(const T value, const T minimum, const T maximum)
    {
        if (value < minimum)
            return minimum;
        if (value > maximum)
            return maximum;
        return value;
    }

    template<typename T>
    static inline constexpr T saturate(const T value, const T maximum)
    {
        if (value < -maximum)
            return -maximum;
        if (value > maximum)
            return maximum;
        return value;
    }

    template<typename T>
    static inline constexpr T ramp(const T value, const T desired, const T limit)
    {
        return value + saturate(desired - value, -limit, limit);
    }

}
