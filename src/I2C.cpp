//
// Created by Matous Hybl on 2018-10-14.
//

#include "roboutils/I2C.h"

#if defined(__linux__)

namespace RoboUtils {

    I2C::I2C(int busNumber) {
        i2cDescriptor = i2c_init(busNumber);
    }

    I2C::~I2C() {
        i2c_close(i2cDescriptor);
    }

    void I2C::write16bitArray(uint8_t chipAddress, uint8_t registerAddress, int16_t *array, uint8_t arraySize) {
        std::lock_guard <std::mutex> lock(mutex);
        int resultLength = i2c_write_leint16_array(i2cDescriptor, chipAddress, registerAddress, array, arraySize);

        // chip address, register address, chip address, array
        if (resultLength != 3 + arraySize) {
            throw "failed to write to i2c";
        }
    }

    void I2C::read32bitArray(uint8_t chipAddress, uint8_t registerAddress, int32_t *array, uint8_t arraySize) {
        std::lock_guard <std::mutex> lock(mutex);
        int resultLength = i2c_read_leint32_array(i2cDescriptor, chipAddress, registerAddress, array, arraySize);
        if (resultLength != arraySize) {
            throw "failed to read from i2c";
        }
    }

    uint16_t I2C::read16bitBEValue(uint8_t chipAddress, uint8_t registerAddress) {
        std::lock_guard <std::mutex> lock(mutex);
        uint16_t value;
        int resultLength = i2c_read_beuint16(i2cDescriptor, chipAddress, registerAddress, &value);

        if (resultLength != 2) {
            throw "failed to read from i2c";
        }
        return value;
    }

    uint16_t I2C::read16bitLEValue(uint8_t chipAddress, uint8_t registerAddress) {
        std::lock_guard <std::mutex> lock(mutex);
        uint16_t value;
        int resultLength = i2c_read_leuint16(i2cDescriptor, chipAddress, registerAddress, &value);

        if (resultLength != 2) {
            throw "failed to read from i2c";
        }
        return value;
    }

    void I2C::write16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t value) {
        std::lock_guard <std::mutex> lock(mutex);
        int resultLength = i2c_write_leuint16(i2cDescriptor, chipAddress, registerAddress, value);

        if (resultLength != 3 + 2) {
            throw "failed to write to i2c";
        }
    }

    void I2C::update16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits,
                                 uint16_t toggleBits) {
        std::lock_guard <std::mutex> lock(mutex);
        int resultLength = i2c_rmw_leuint16(i2cDescriptor, chipAddress, registerAddress, clearBits, setBits,
                                            toggleBits);

        if (resultLength != 3) {
            throw "failed to write to i2c";
        }
    }
};

#endif
