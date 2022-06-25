/*
 MIT License

Copyright (c) 2019-2021 Matous Hybl
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

#include <roboutils/io/GPIO.h>

#include <roboutils/chips/mcp23017.h>


using namespace RoboUtils::IO;
using namespace RoboUtils::Chips;

GPIO::GPIO(const I2C &i2c, int chipIndex)
  : i2c_(i2c), chipAddress_(Mcp23017::Addr(chipIndex))
{
}

GPIO::operator bool() const
{
    uint8_t reg;
    return i2c_.read(chipAddress_, +Mcp23017::Reg::IODIR, &reg);
}

const I2C &GPIO::bus() const
{
    return i2c_;
}

int GPIO::chip() const
{
    return chipAddress_;
}

void GPIO::set_chip(int address)
{
  chipAddress_ = address;
}

void GPIO::input(uint16_t pins, bool pullup) const
{
    if (!i2c_.update<uint16_t>(chipAddress_, +Mcp23017::Reg::IODIR, pins, 0, 0,Endian::Little))
        throw gpio_error();

    if (!i2c_.update<uint16_t>(chipAddress_, +Mcp23017::Reg::GPPU, pullup ? pins : 0, pullup ? 0 : pins, 0,Endian::Little))
        throw gpio_error();
}

void GPIO::output(uint16_t pins) const
{
    if (!i2c_.update<uint16_t>(chipAddress_, +Mcp23017::Reg::IODIR, 0, pins, 0,Endian::Little))
        throw gpio_error();
}

void GPIO::set(uint16_t pins, bool value) const
{
    if (!i2c_.update<uint16_t>(chipAddress_,  +Mcp23017::Reg::OLAT, value ? 0 : pins, value ? pins : 0, 0, Endian::Little))
        throw gpio_error();
}

void GPIO::low(uint16_t pins) const
{
    if (!i2c_.update<uint16_t>(chipAddress_,  +Mcp23017::Reg::OLAT, pins, 0, 0, Endian::Little))
        throw gpio_error();
}

void GPIO::high(uint16_t pins) const
{
    if (!i2c_.update<uint16_t>(chipAddress_,  +Mcp23017::Reg::OLAT, 0, pins, 0, Endian::Little))
        throw gpio_error();
}

void GPIO::toggle(uint16_t pins) const
{
    if (!i2c_.update<uint16_t>(chipAddress_,  +Mcp23017::Reg::OLAT, 0, 0, pins, Endian::Little))
        throw gpio_error();
}

bool GPIO::get(uint16_t pins) const
{
    uint16_t p = 0;
    if (!i2c_.read(chipAddress_, +Mcp23017::Reg::GPIO, &p, Endian::Little))
        throw gpio_error();

    return (pins & p) != 0;
}

uint16_t GPIO::read(uint16_t pins) const
{
    uint16_t p = 0;
    if (!i2c_.read(chipAddress_, +Mcp23017::Reg::GPIO, &p, Endian::Little))
        throw gpio_error();

    return p & pins;
}