//
// Created by Matous Hybl on 2019-01-05.
//

#ifndef FIRMWARE_GPIO_H
#define FIRMWARE_GPIO_H

#if defined(__linux__)
#include "I2C.h"

namespace RoboUtils {
#define INPUT true
#define OUTPUT false
#define HIGH true
#define LOW false
#define PULLUP_ON true
#define PULLUP_OFF false

// C++ wrapper around MCP3017 library
    ///-------------------------------------------------------------------------------------------------------------
    /// \brief The GPIO multi-threaded library
    ///
    /// \code
    ///  I2C bus{"/dev/i2c-1"};
    ///  GPIO gpio{ &bus };
    ///
    ///  gpio.output(Pin::PA1 | Pin::PA2 | Pin::PA3);        // set PA1...PA3 to output
    ///  gpio.input_pullup(Pin::PA7 | Pin::PA6);             // set pins PA7 and PA6 to input
    ///
    ///  while(gpio.get(Pin::PA7)) {                         // loo until button on PA7 press
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
        explicit GPIO(I2C *i2c, int chipIndex = 0);

        void setDirection(uint16_t pin, bool input);

        void setPullUp(uint16_t pin, bool input);

        void write(uint16_t pin, bool input);

        bool read(uint16_t pin);

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set selected pins to input mode
        ///
        /// \param pins
        void input(uint16_t pins) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Set specified pins to input mode with active pullup
        ///
        /// \param pins
        void input_pullup(uint16_t pins) const;

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
        /// \return
        bool get(uint16_t pins) const;

    private:
        /// the bus
        I2C *i2c = nullptr;

        /// original chip address
        int chipAddress;
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
};

#endif
#endif //FIRMWARE_GPIO_H
