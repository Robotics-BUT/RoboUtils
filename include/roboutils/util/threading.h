//
// Created by Matous Hybl on 2018-11-14.
//

#pragma once

#include <functional>

namespace RoboUtils {

    /// \brief Call this function periodically from other thread
    ///
    /// when the function returns false, the thread stops.
    ///
    /// The time behavior is undefined when the duration of the function is longer than the period
    ///
    /// \code
    ///   repeatAsynchronously(100, [](){ printf("I still live !\r\n"); return true; });
    /// \endcode
    /// \param periodMs period to call the function
    /// \param fun the function to call
    void repeatAsynchronously(unsigned int periodMs, const std::function<bool(void)> &fun);

}

