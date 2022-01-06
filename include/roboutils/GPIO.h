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
        explicit GPIO(I2C *i2c, int chipIndex = 0);

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

    namespace Pin {
        static const uint16_t PB7 = (1 << 15);
        static const uint16_t PB6 = (1 << 14);
        static const uint16_t PB5 = (1 << 13);
        static const uint16_t PB4 = (1 << 12);
        static const uint16_t PB3 = (1 << 11);
        static const uint16_t PB2 = (1 << 10);
        static const uint16_t PB1 = (1 << 9);
        static const uint16_t PB0 = (1 << 8);
        static const uint16_t PA7 = (1 << 7);
        static const uint16_t PA6 = (1 << 6);
        static const uint16_t PA5 = (1 << 5);
        static const uint16_t PA4 = (1 << 4);
        static const uint16_t PA3 = (1 << 3);
        static const uint16_t PA2 = (1 << 2);
        static const uint16_t PA1 = (1 << 1);
        static const uint16_t PA0 = (1 << 0);
    }
};

#endif
#endif //FIRMWARE_GPIO_H
