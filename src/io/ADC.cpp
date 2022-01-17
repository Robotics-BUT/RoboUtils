/*
 MIT License

Copyright (c) 2018-2021 Matous Hybl
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

#include "roboutils/io/ADC.h"

#include "roboutils/chips/ad799x.h"

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils::IO;
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