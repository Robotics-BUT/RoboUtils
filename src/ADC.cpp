//
// Created by Matous Hybl on 2018-10-23.
//

#include <cstdint>
#include "roboutils/ADC.h"
#include "roboutils/chips/ad799x.h"

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils;
using namespace RoboUtils::Chips;

ADC::ADC(I2C *i2c)
{
    this->i2c = i2c;
    chipAddress = ADDR_AD799X_0_L;
}

uint16_t ADC::readChannel(uint8_t channel)
{

    uint16_t channelAndValue;
    if (!i2c->read(chipAddress, Ad799X::RESULT::Reg(channel), &channelAndValue, false))
        throw adc_error();

    uint16_t readChannel = Ad799X::RESULT::FromCHAN(channelAndValue);

    // FIXME test if actually true
    if (readChannel != channel)
        throw std::logic_error("ADC: received result channel is not equal to requested channel");

    return Ad799X::RESULT::FromVALUE(channelAndValue );
}