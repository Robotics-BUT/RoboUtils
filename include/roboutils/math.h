//
// Created by Matous Hybl on 02/10/2018.
//

#include <cstdint>

#ifndef FIRMWARE_MATH_H
#define FIRMWARE_MATH_H

namespace RoboUtils {
#define PI 3.14159f

// FIXME rewrite using templates
    inline int16_t saturate(const int16_t value, const int16_t minimum, const int16_t maximum) {
        if (value < minimum) {
            return minimum;
        }

        if (value > maximum) {
            return maximum;
        }

        return value;
    }

    inline float saturatef(const float value, const float minimum, const float maximum) {
        if (value < minimum) {
            return minimum;
        }

        if (value > maximum) {
            return maximum;
        }

        return value;
    }

};

#endif //FIRMWARE_MATH_H
