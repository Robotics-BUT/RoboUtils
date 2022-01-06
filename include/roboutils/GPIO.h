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
    class GPIO {
    public:
        explicit GPIO(I2C *i2c, int aChipAddress = 0);

        void setDirection(uint16_t pin, bool input);

        void setPullUp(uint16_t pin, bool input);

        void write(uint16_t pin, bool input);

        bool read(uint16_t pin);

        // new API

        void input(uint16_t pins) const;

        void input_pullup(uint16_t pins) const;

        void output(uint16_t pins) const;

        void set(uint16_t pins, bool value) const;

        void low(uint16_t pins) const;

        void high(uint16_t pins) const;

        void toggle(uint16_t pins) const;

        bool get(uint16_t pins) const;

    private:
        I2C *i2c = nullptr;
        int chipAddress;
    };
};

#endif
#endif //FIRMWARE_GPIO_H
