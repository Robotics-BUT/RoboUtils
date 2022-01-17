//
// Created by Matous Hybl on 2019-01-05.
//

#include "roboutils/io/GPIO.h"

#include "roboutils/chips/mcp23017.h"

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils::IO;
using namespace RoboUtils::Chips;

GPIO::GPIO(I2C *i2c, int chipIndex)
{
    this->i2c = i2c;
    chipAddress = Mcp23017::Addr(chipIndex);
}

void GPIO::input(uint16_t pins, bool pullup) const
{
    if (!i2c->update<uint16_t>(chipAddress, +Mcp23017::Reg::IODIR, pins, 0, 0, true))
        throw gpio_error();

    if (!i2c->update<uint16_t>(chipAddress, +Mcp23017::Reg::GPPU, pullup ? pins : 0, pullup ? 0 : pins, 0, true))
        throw gpio_error();
}

void GPIO::output(uint16_t pins) const
{
    if (!i2c->update<uint16_t>(chipAddress, +Mcp23017::Reg::IODIR, 0, pins, 0, true))
        throw gpio_error();
}

void GPIO::set(uint16_t pins, bool value) const
{
    if (!i2c->update<uint16_t>(chipAddress,  +Mcp23017::Reg::OLAT, value ? 0 : pins, value ? pins : 0, 0, true))
        throw gpio_error();
}

void GPIO::low(uint16_t pins) const
{
    if (!i2c->update<uint16_t>(chipAddress,  +Mcp23017::Reg::OLAT, pins, 0, 0, true))
        throw gpio_error();
}

void GPIO::high(uint16_t pins) const
{
    if (!i2c->update<uint16_t>(chipAddress,  +Mcp23017::Reg::OLAT, 0, pins, 0, true))
        throw gpio_error();
}

void GPIO::toggle(uint16_t pins) const
{
    if (!i2c->update<uint16_t>(chipAddress,  +Mcp23017::Reg::OLAT, 0, 0, pins, true))
        throw gpio_error();
}

bool GPIO::get(uint16_t pins) const
{
    uint16_t p = 0;
    if (!i2c->read(chipAddress, +Mcp23017::Reg::GPIO, &p, true))
        throw gpio_error();

    return (pins & p) != 0;
}
