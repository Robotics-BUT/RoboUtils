#pragma once
/*
 MIT License

Copyright (c) 2022 Frantisek Burian

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <tuple>

#include <roboutils/io/I2C.h>

namespace RoboUtils::IO {

    class KM2 {
    public:
        explicit KM2(const I2C &aBus, int aChipAddress = 0x71);

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief operator that informs if bus is already opened
        ///
        /// \return true, if bus is correctly opened
        operator bool() const;

        /// \brief Set desired speed for both of the motors
        ///
        /// \param left left motor speed
        /// \param right right motor speed
        void drive(int left, int right) const;

        /// \brief Ask for odometry
        ///
        /// \code
        ///   auto [ left, right ] = odo();
        /// \endcode
        ///
        /// \return current odometry measurement
        std::tuple<int,int> odometry() const;

        /// \brief Set desired speed for both of the motors and ask for odometry
        ///
        /// \code
        ///   auto [ left, right ] = driveodo(3,5);
        /// \endcode
        ///
        /// \param left left motor speed
        /// \param right right motor speed
        /// \return current odometry measurement
        std::tuple<int,int> driveodo(int left, int right) const;

        void setAddress(int newaddr, bool bcast) const;

    private:
        const I2C &bus_;

        const int chipAddress_;
    };

    class km2_error : public std::logic_error {
    public:
        km2_error() : std::logic_error("ADC: Access error") {}
    };

}

