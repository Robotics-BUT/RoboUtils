#pragma once
/*
 MIT License

Copyright (c) 2018-2021 Matous Hybl
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

#include <roboutils/io/I2C.h>

#include <vector>
#include <map>

namespace RoboUtils::IO {

    class ADC {
    public:
        explicit ADC(const I2C &i2c);

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief operator that informs if bus is already opened
        ///
        /// \return true, if bus is correctly opened
        operator bool() const;

        /// \brief Measure single value on ADC channel
        ///
        /// \param channel the channel number (0-7)
        /// \return the map with measured value
        std::map<int, uint16_t> Mode2Measure(int channel);

        /// \brief Measure multiple values on multiple channels
        ///
        /// \param channels all channels to be measured
        /// \return the map set up with measured values
        std::map<int, uint16_t> Mode2Measure(const std::vector<int>& channels);

        void setCycleMode(int divisor);

    private:
        const I2C &i2c_;

        const int chipAddress_;
    };

    class adc_error : public std::logic_error {
    public:
        adc_error() : std::logic_error("ADC: Access error") {}
    };
}

