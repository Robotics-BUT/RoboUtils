//
// Created by Matous Hybl on 2019-01-05.
//

#include <cstdint>
#include "roboutils/GPIO.h"
#include "roboutils/mcp23017.h"

#if defined(__linux__)

namespace RoboUtils {
    GPIO::GPIO(I2C *i2c) {
        this->i2c = i2c;
    }

    void GPIO::setDirection(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                ADDR_MCP23017_A(0),
                MCP23017_IODIR,
                static_cast<uint16_t>(input ? pin : 0),
                static_cast<uint16_t>(input ? 0 : pin),
                0);
    }

    void GPIO::write(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                ADDR_MCP23017_A(0),
                MCP23017_OLAT,
                static_cast<uint16_t>(input ? 0 : pin),
                static_cast<uint16_t>(input ? pin : 0),
                0);
    }

    bool GPIO::read(uint16_t pin) {
        uint16_t registerValue = i2c->read16bitLEValue(ADDR_MCP23017_A(0), MCP23017_GPIO);

        return (registerValue & pin) != 0;
    }

    void GPIO::setPullUp(uint16_t pin, bool input) {
        i2c->update16bitLEValue(
                ADDR_MCP23017_A(0),
                MCP23017_GPPU,
                static_cast<uint16_t>(input ? pin : 0),
                static_cast<uint16_t>(input ? 0 : pin),
                0);
    }
};
#endif
