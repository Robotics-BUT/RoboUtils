//
// Created by Matous Hybl on 2018-10-23.
//

#ifndef FIRMWARE_ADC_H
#define FIRMWARE_ADC_H

#include "roboutils/I2C.h"

namespace RoboUtils {

    class ADC {
    public:
        explicit ADC(I2C *i2c);

        uint16_t readChannel(uint8_t channel);

    private:
        I2C *i2c;

        int chipAddress;
    };

    class adc_error : public std::logic_error {
    public:
        adc_error() : std::logic_error("ADC: Access error") {}
    };
};

#endif



