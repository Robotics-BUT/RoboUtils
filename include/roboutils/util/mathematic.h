//
// Created by Matous Hybl on 02/10/2018.
//
#pragma once

#include <cstdint>

namespace RoboUtils {

    /// \brief Saturate value between minimum and maximum
    ///
    /// \code
    ///   int64_t value = 100000;
    ///   value = saturate(value, -200, 100);   // = 100
    /// \endcode
    ///
    /// \tparam T any countable value type (ints or floats of any size)
    /// \param value the value to be saturated
    /// \param minimum lower limit
    /// \param maximum upper limit
    /// \return the saturated value
    template<typename T>
    static inline constexpr T saturate(const T value, const T minimum, const T maximum)
    {
        if (value < minimum)
            return minimum;
        if (value > maximum)
            return maximum;
        return value;
    }

    /// \brief Saturate value to symmetrical limit
    ///
    /// \code
    ///   int64_t value = -100000;
    ///   value = saturate(value, 100);   // = -100
    /// \endcode
    ///
    /// \tparam T any countable value type (ints or floats of any size)
    /// \param value the value to be saturated
    /// \param maximum the limit
    /// \return the saturated value
    template<typename T>
    static inline constexpr T saturate(const T value, const T maximum)
    {
        if (value < -maximum)
            return -maximum;
        if (value > maximum)
            return maximum;
        return value;
    }

    /// \brief Tracking speed limiter
    ///
    /// \tparam T any countable value type (ints or floats of any size)
    /// \param value actual value
    /// \param desired the desired value the user wants to have
    /// \param limit the maximum step per regulation
    /// \return saturated actual value
    template<typename T>
    static inline constexpr T ramp(const T value, const T desired, const T limit)
    {
        return value + saturate(desired - value, -limit, limit);
    }

}
