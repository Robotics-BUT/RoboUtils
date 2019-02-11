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
        GPIO(I2C *i2c);

        void setDirection(uint16_t pin, bool input);

        void setPullUp(uint16_t pin, bool input);

        void write(uint16_t pin, bool input);

        bool read(uint16_t pin);

    private:
        I2C *i2c = nullptr;
    };
};

#endif
#endif //FIRMWARE_GPIO_H
