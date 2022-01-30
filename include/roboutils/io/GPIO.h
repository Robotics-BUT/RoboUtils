#pragma once
/*
 MIT License

Copyright (c) 2019-2021 Matous Hybl
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

namespace RoboUtils::IO {

    ///-------------------------------------------------------------------------------------------------------------
    /// \brief The GPIO multi-threaded library
    ///
    /// \code
    ///  auto RUN = Pin::PA7;
    ///
    ///  I2C bus;
    ///  GPIO gpio{bus};
    ///
    ///  bus.open("/dev/i2c-1");
    ///
    ///  gpio.output(Pin::PA1 | Pin::PA2 | Pin::PA3);        // set PA1...PA3 to output
    ///  gpio.input(RUN | Pin::PA6, true);                   // set pins PA7 and PA6 to input with pullup
    ///
    ///  while(gpio.get(RUN)) {                              // loo until button on PA7 press
    ///      gpio.high(Pin::PA1);                            // set PA1 to high level (LED off)
    ///      delay(500);
    ///      gpio.low(Pin::PA1);                             // set PA1 to low level (LED on)
    ///      delay(500);
    ///      gpio.toggle(Pin::PA2);                          // change value of PA2 (blink)
    ///      gpio.set(Pin::PA3, gpio.get(Pin::PA6));         // copy value of PA6 to PA3
    ///  }
    /// \endcode
    class GPIO {
    public:

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Constructor of interface
        ///
        /// \param i2c the bus to communicate over
        /// \param chipIndex the index of the chip on bus
        explicit GPIO(const I2C &i2c, int chipIndex = 0);

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief operator that informs if bus is already opened
        ///
        /// \return true, if chip is already present
        operator bool() const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Associated bus
        ///
        /// \return the associated bus
        const I2C &bus() const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Chip address
        ///
        /// \return the current chip address
        int chip() const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set selected pins to input mode
        ///
        /// \param pins
        /// \param pullup true if the pullup should e activated on those pins
        void input(uint16_t pins, bool pullup = true) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set specified pins to output mode
        ///
        /// \param pins
        void output(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set specified pins to specified level
        ///
        /// \param pins
        /// \param value
        void set(uint16_t pins, bool value) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set specified pins to low level (0V)
        ///
        /// \param pins
        void low(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set specified pins to high level (3.3V)
        ///
        /// \param pins
        void high(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Toggle specified pins
        ///
        /// \param pins
        void toggle(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Get actual pin value
        ///
        /// \param pins
        /// \return true when pin was at high state
        bool get(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Get actual port values
        ///
        /// \param pins which pins should be read
        /// \return set of pins which was at high state
        uint16_t read(uint16_t pins) const;


    private:
        /// the bus
        const I2C &i2c_;

        /// original chip address
        const int chipAddress_;
    };

    class gpio_error : public std::logic_error {
    public:
        gpio_error() : std::logic_error("GPIO: Access error") {}
    };

    namespace Pin {

        enum : uint16_t {
             PB7 = (1 << 15),
             PB6 = (1 << 14),
             PB5 = (1 << 13),
             PB4 = (1 << 12),
             PB3 = (1 << 11),
             PB2 = (1 << 10),
             PB1 = (1 << 9),
             PB0 = (1 << 8),
             PA7 = (1 << 7),
             PA6 = (1 << 6),
             PA5 = (1 << 5),
             PA4 = (1 << 4),
             PA3 = (1 << 3),
             PA2 = (1 << 2),
             PA1 = (1 << 1),
             PA0 = (1 << 0),
        };

    }
}
