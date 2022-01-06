//
// Created by Matous Hybl on 2019-01-05.
//

#include <cstdint>
#include "roboutils/GPIO.h"
#include "roboutils/mcp23017.h"

#if defined(__linux__)

using namespace RoboUtils;
using namespace RoboUtils::Chips::Mcp23017;

GPIO::GPIO(I2C *i2c, int chipIndex) {
    this->i2c = i2c;
    chipAddress = Addr(chipIndex);
}

void GPIO::setDirection(uint16_t pin, bool input) {
    i2c->update16bitLEValue(
            chipAddress,
            +Reg::IODIR,
            static_cast<uint16_t>(input ? pin : 0),
            static_cast<uint16_t>(input ? 0 : pin),
            0);
}

void GPIO::write(uint16_t pin, bool input) {
    i2c->update16bitLEValue(
            chipAddress,
            +Reg::OLAT,
            static_cast<uint16_t>(input ? 0 : pin),
            static_cast<uint16_t>(input ? pin : 0),
            0);
}

bool GPIO::read(uint16_t pin) {
    uint16_t registerValue = i2c->read16bitLEValue(chipAddress, +Reg::GPIO);

    return (registerValue & pin) != 0;
}

void GPIO::setPullUp(uint16_t pin, bool input) {
    i2c->update16bitLEValue(
            chipAddress,
            +Reg::GPPU,
            static_cast<uint16_t>(input ? pin : 0),
            static_cast<uint16_t>(input ? 0 : pin),
            0);
}

//----------------------------------------------------------------------------------------------------------------
// new api

void GPIO::input(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress, +Reg::IODIR, 0, pins, 0);
    i2c->update16bitLEValue(chipAddress, +Reg::GPPU, pins, 0, 0);
}

void GPIO::input_pullup(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress, +Reg::IODIR, 0, pins, 0);
    i2c->update16bitLEValue(chipAddress, +Reg::GPPU, 0, pins, 0);
}

void GPIO::output(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress, +Reg::IODIR, pins, 0, 0);
}

void GPIO::set(uint16_t pins, bool value) const
{
    i2c->update16bitLEValue(chipAddress,  +Reg::OLAT, value ? 0 : pins, value ? pins : 0, 0);
}

void GPIO::low(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress,  +Reg::OLAT, pins, 0, 0);
}

void GPIO::high(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress,  +Reg::OLAT, 0, pins, 0);
}

void GPIO::toggle(uint16_t pins) const
{
    i2c->update16bitLEValue(chipAddress,  +Reg::OLAT, 0, 0, pins);
}

bool GPIO::get(uint16_t pins) const
{
    uint16_t p = 0;
    if (!i2c->readLe(chipAddress, +Reg::GPIO, &p))
        throw "failed to get GPIO state";

    return (pins & p) != 0;
}

#endif
