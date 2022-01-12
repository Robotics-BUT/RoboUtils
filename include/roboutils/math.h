//
// Created by Matous Hybl on 02/10/2018.
//

#include <cstdint>

#ifndef FIRMWARE_MATH_H
#define FIRMWARE_MATH_H

namespace RoboUtils {
#define PI 3.14159f

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

};

#endif //FIRMWARE_MATH_H
