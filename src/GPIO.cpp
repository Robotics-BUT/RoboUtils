//
// Created by Matous Hybl on 2019-01-05.
//

#include <cstdint>
#include "roboutils/GPIO.h"
#include "roboutils/mcp23017.h"

#if defined(__linux__)

namespace RoboUtils {
    GPIO::GPIO(I2C *i2c, int aChipAddress) {
        this->i2c = i2c;
        chipAddress = ADDR_MCP23017_A(aChipAddress);
    }

    void GPIO::setDirection(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                chipAddress,
                MCP23017_IODIR,
                static_cast<uint16_t>(input ? pin : 0),
                static_cast<uint16_t>(input ? 0 : pin),
                0);
    }

    void GPIO::write(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                chipAddress,
                MCP23017_OLAT,
                static_cast<uint16_t>(input ? 0 : pin),
                static_cast<uint16_t>(input ? pin : 0),
                0);
    }

    bool GPIO::read(uint16_t pin) {
        uint16_t registerValue = i2c->read16bitLEValue(chipAddress, MCP23017_GPIO);

        return (registerValue & pin) != 0;
    }

    void GPIO::setPullUp(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                chipAddress,
                MCP23017_GPPU,
                static_cast<uint16_t>(input ? pin : 0),
                static_cast<uint16_t>(input ? 0 : pin),
                0);
    }

    //----------------------------------------------------------------------------------------------------------------
    // new api

    void GPIO::input(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress, MCP23017_IODIR, 0, pins, 0);
        i2c->update16bitLEValue(chipAddress, MCP23017_GPPU, pins, 0, 0);
    }

    void GPIO::input_pullup(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress, MCP23017_IODIR, 0, pins, 0);
        i2c->update16bitLEValue(chipAddress, MCP23017_GPPU, 0, pins, 0);
    }

    void GPIO::output(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress, MCP23017_IODIR, pins, 0, 0);
    }

    void GPIO::set(uint16_t pins, bool value) const
    {
        i2c->update16bitLEValue(chipAddress,  MCP23017_OLAT, value ? 0 : pins, value ? pins : 0, 0);
    }

    void GPIO::low(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress,  MCP23017_OLAT, pins, 0, 0);
    }

    void GPIO::high(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress,  MCP23017_OLAT, 0, pins, 0);
    }

    void GPIO::toggle(uint16_t pins) const
    {
        i2c->update16bitLEValue(chipAddress,  MCP23017_OLAT, 0, 0, pins);
    }

    bool GPIO::get(uint16_t pins) const
    {
        uint16_t p = 0;
        if (!i2c->readLe(chipAddress, MCP23017_GPIO, &p))
            throw "failed to get GPIO state";

        return (pins & p) != 0;
    }
};
#endif
