//
// Created by Matous Hybl on 2018-10-23.
//

#ifndef FIRMWARE_ADC_H
#define FIRMWARE_ADC_H

#if defined(__linux__)

#include "I2C.h"

namespace RoboUtils {
    class ADC {
    public:
        explicit ADC(I2C *i2c);

        uint16_t readChannel(uint8_t channel);

    private:
        I2C *i2c;

        int chipAddress;
    };
};

#endif


#endif //FIRMWARE_ADC_H
