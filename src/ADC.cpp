//
// Created by Matous Hybl on 2018-10-23.
//

#include <cstdint>
#include "roboutils/ADC.h"
#include "roboutils/chips/ad799x.h"

#if defined(__linux__)
namespace RoboUtils {

    using namespace Chips::Ad799X;

    ADC::ADC(I2C *i2c) {
        this->i2c = i2c;
        chipAddress = ADDR_AD799X_0_L;
    }

    uint16_t ADC::readChannel(uint8_t channel) {

        uint16_t channelAndValue;
        if (!i2c->readBe(chipAddress, RESULT::Reg(channel), &channelAndValue))
            throw "Can't read from ADC";

        uint16_t readChannel = RESULT::FromCHAN(channelAndValue);

        // FIXME test if actually true
        if (readChannel != channel)
            throw "ADC: received result channel is not equal to requested channel";

        return RESULT::FromVALUE(channelAndValue );
    }
};

#endif