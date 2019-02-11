//
// Created by Matous Hybl on 2018-10-14.
//

#ifndef FIRMWARE_I2C_H
#define FIRMWARE_I2C_H

#if defined(__linux__)

#include <mutex>

namespace RoboUtils {
    class I2C {
    public:
        I2C(int busNumber = 1);

        ~I2C();

        void write16bitArray(uint8_t chipAddress, uint8_t registerAddress, int16_t array[], uint8_t arraySize);

        void read32bitArray(uint8_t chipAddress, uint8_t registerAddress, int32_t *array, uint8_t arraySize);

        uint16_t read16bitBEValue(uint8_t chipAddress, uint8_t registerAddress);

        uint16_t read16bitLEValue(uint8_t chipAddress, uint8_t registerAddress);

        void write16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t value);

        void update16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits,
                                uint16_t toggleBits);

    private:
        int i2cDescriptor;
        std::mutex mutex;
    };
};

#endif


#endif //FIRMWARE_I2C_H
