//
// Created by Matous Hybl on 2018-10-23.
//

#include <cstdint>
#include "roboutils/ADC.h"

#if defined(__linux__)
namespace RoboUtils {
    ADC::ADC(I2C *i2c) {
        this->i2c = i2c;
    }

    uint16_t ADC::readChannel(uint8_t channel) {
        uint16_t channelAndValue = i2c->read16bitBEValue(ADDR_AD799X_0_L, AD799X_RESULT_CH(channel));
        uint16_t readChannel = (channelAndValue & AD799X_RESULT_CHAN) >> 12;
        uint16_t value = (channelAndValue & AD799X_RESULT_VALUE);

        // FIXME test if actually true
        if (readChannel != channel) {
            throw "ADC: received result channel is not equal to requested channel";
        }

        return value;
    }
};

#endif