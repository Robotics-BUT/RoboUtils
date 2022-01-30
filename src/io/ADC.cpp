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

#include <roboutils/io/ADC.h>

#include <roboutils/chips/ad799x.h>

using namespace RoboUtils::IO;
using namespace RoboUtils::Chips;

ADC::ADC(const I2C &i2c)
 : i2c_(i2c), chipAddress_(ADDR_AD799X_0_L)
{
}

ADC::operator bool() const
{
    return i2c_;
}

const I2C &ADC::bus() const
{
    return i2c_;
}

int ADC::chip() const
{
    return chipAddress_;
}

// mode 2
std::map<int, uint16_t> ADC::Mode2Measure(int channel)
{
    uint16_t reg;
    if (!i2c_.read(chipAddress_, Ad799X::RESULT::Reg(channel), &reg, Endian::Big))
        throw adc_error();

    return { {
        Ad799X::RESULT::FromCHAN(reg),
        Ad799X::RESULT::FromVALUE(reg) }
    };
}

std::map<int, uint16_t> ADC::Mode2Measure(const std::vector<int>& channels)
{
    std::map<int, uint16_t> map;

#if 1
    uint16_t reg;

    for (auto channel : channels) {

        if (!i2c_.read(chipAddress_, Ad799X::RESULT::Reg(channel), &reg, Endian::Big))
            throw adc_error();

        map.insert({
               Ad799X::RESULT::FromCHAN(reg),
               Ad799X::RESULT::FromVALUE(reg)
       });
    }

#else
    // FIXME nasledujici kod by mel fungovat na dva i2c trnsacty dle DS ale nejede. Zrejme nepublikovana errata

    std::vector<uint16_t> data(channels.size(), 0);
    uint16_t cfg = 0;//Ad799X::CONFIG::FLTR;

    for (auto channel : channels)
        cfg |= Ad799X::CONFIG::ToCh(channel);

    if (!i2c_.write(chipAddress_, +Ad799X::Reg::CONFIG, cfg, false))
        throw adc_error();

    if (!i2c_.read(chipAddress_, +Ad799X::Reg::RESULT_SEQ, data.data(), (int)data.size(), false))
        throw adc_error();

    for (auto i : data)
        map.insert({
            Ad799X::RESULT::FromCHAN(i),
            Ad799X::RESULT::FromVALUE(i)
        });
#endif

    return map;
}

void ADC::setCycleMode(int divisor)
{
    if (!i2c_.write(chipAddress_, +Ad799X::Reg::CYCLE, (uint8_t)divisor, Endian::Big))
        throw adc_error();
}