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
        explicit I2C(const std::string &busFile = "/dev/i2c-1");

        ~I2C();

        template<typename T> bool writeLe(int chipAddress, int registerAddress, T value) const;

        template<typename T> bool writeBe(int chipAddress, int registerAddress, T value) const;

        template<typename T> bool writeLe(int chipAddress, int registerAddress, const T *array, int count) const;

        template<typename T> bool writeBe(int chipAddress, int registerAddress, const T *array, int count) const;

        template<typename T> bool readLe(int chipAddress, int registerAddress, T *value) const;

        template<typename T> bool readBe(int chipAddress, int registerAddress, T *value) const;

        template<typename T> bool readLe(int chipAddress, int registerAddress, T *array, int count) const;

        template<typename T> bool readBe(int chipAddress, int registerAddress, T *array, int count) const;

        void write16bitArray(uint8_t chipAddress, uint8_t registerAddress, int16_t array[], uint8_t arraySize);

        void read32bitArray(uint8_t chipAddress, uint8_t registerAddress, int32_t *array, uint8_t arraySize);

        uint16_t read16bitBEValue(uint8_t chipAddress, uint8_t registerAddress);

        uint16_t read16bitLEValue(uint8_t chipAddress, uint8_t registerAddress);

        void write16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t value);

        void update16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits,
                                uint16_t toggleBits);

    private:
        int i2cDescriptor;
        mutable std::mutex mutex;

        bool transact_(int addr, uint8_t *w, uint32_t wn, uint8_t *r, uint32_t rn) const;

        bool writeBe_(int chipAddress, int registerAddress,  const uint8_t *data, int size, int count = 1) const;

        bool writeLe_(int chipAddress, int registerAddress,  const uint8_t *data, int size, int count = 1) const;

        bool readBe_(int chipAddress, int registerAddress,  uint8_t *data, int size, int count = 1) const;

        bool readLe_(int chipAddress, int registerAddress,  uint8_t *data, int size, int count = 1) const;
    };
};

#endif


#endif //FIRMWARE_I2C_H
