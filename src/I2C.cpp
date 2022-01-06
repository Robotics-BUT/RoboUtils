//
// Created by Matous Hybl on 2018-10-14.
//
#include <cstdio>

#include "roboutils/I2C.h"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstdint>
#include <unistd.h>

#if defined(__linux__)

using namespace RoboUtils;

I2C::I2C(const std::string &busFile) {
    i2cDescriptor = open(busFile.c_str(), O_RDWR);
}

I2C::~I2C() {
    close(i2cDescriptor);
}

void I2C::write16bitArray(uint8_t chipAddress, uint8_t registerAddress, int16_t *array, uint8_t arraySize) {

    if (!writeLe(chipAddress, registerAddress, array, arraySize))
        throw "failed to write to i2c";
}

void I2C::read32bitArray(uint8_t chipAddress, uint8_t registerAddress, int32_t *array, uint8_t arraySize) {

    if (!readLe(chipAddress, registerAddress, array, arraySize))
        throw "failed to read from i2c";
}

uint16_t I2C::read16bitBEValue(uint8_t chipAddress, uint8_t registerAddress) {

    uint16_t value;
    if (!readBe(chipAddress, registerAddress, &value))
        throw "failed to read from i2c";

    return value;
}

uint16_t I2C::read16bitLEValue(uint8_t chipAddress, uint8_t registerAddress) {
    uint16_t value;
    if (!readLe(chipAddress, registerAddress, &value))
        throw "failed to read from i2c";

    return value;
}

void I2C::write16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t value) {

    if (!writeLe(chipAddress, registerAddress, value))
        throw "failed to write to i2c";
}


void I2C::update16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits,
                             uint16_t toggleBits) {
    uint16_t value;

    if (!readLe(chipAddress, registerAddress, &value))
        throw "failed to read from i2c";

    value = ((value & ~clearBits) | setBits) ^ toggleBits;

    if (!writeLe(chipAddress, registerAddress, value))
        throw "failed to write to i2c";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEW API

template <typename T>
bool I2C::writeLe(const int chipAddress, const int registerAddress, const T value) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(&value), sizeof(T), 1, true);
}

template <typename T>
bool I2C::writeBe(const int chipAddress, const int registerAddress, const T value) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(&value), sizeof(T), 1, false);
}

template <typename T>
bool I2C::writeLe(const int chipAddress, const int registerAddress, const T* array, const int count) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(array), sizeof(T), count, true);
}

template <typename T>
bool I2C::writeBe(const int chipAddress, const int registerAddress, const T* array, const int count) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(array), sizeof(T), count, false);
}

template <typename T>
bool I2C::readLe(const int chipAddress, const int registerAddress, T* value) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(value), sizeof(T), 1, true);
}

template <typename T>
bool I2C::readBe(const int chipAddress, const int registerAddress, T* value) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(value), sizeof(T), 1, false);
}

template <typename T>
bool I2C::readLe(const int chipAddress, const int registerAddress, T* array, const int count) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(array), sizeof(T), count, true);
}

template <typename T>
bool I2C::readBe(const int chipAddress, const int registerAddress, T* array, const int count) const
{
    return read_( chipAddress, registerAddress, reinterpret_cast<uint8_t *>(array), sizeof(T), count, false);
}

//======================================================================================================================
// PRIVATE

//#define LOG // TODO log later using matous's engine
bool I2C::transact_(int addr, uint8_t *w, uint32_t wn, uint8_t *r, uint32_t rn) const
{
    std::lock_guard <std::mutex> lock(mutex);

#ifdef LOG
    printf( "I2C: @%02x TRANSN W %d R %d ", addr, wn, rn);
    if (wn > 0) {
        printf("[ ");
        for (int i = 0 ; i < wn ; ++i)
            printf("%02x ", w[i]);
        printf("] ");
    }
#endif

    if (ioctl(i2cDescriptor, I2C_SLAVE, addr))
        return false;

    if (wn > 0) {
        int err = write(i2cDescriptor, w, wn);


#ifdef LOG
        printf("(%d written) ", err);
#endif
        if (err != wn)
            return false;
    }

    if (rn > 0) {
        int err = read(i2cDescriptor, r, rn);

#ifdef LOG
        printf("(%d readed) [ ", err);
        for (int i = 0 ; i < rn ; ++i)
            printf("%2x ", r[i]);
        printf("]\n");
#endif
        return err == rn;
    }

#ifdef LOG
    printf("\n");
#endif
    return true;
}

bool I2C::write_(int chipAddress, int registerAddress,  const uint8_t *data, int typeSize, int size, bool litleEndian  ) const
{
    uint8_t buff[1 + size * typeSize];
    buff[0] = registerAddress;

    if (litleEndian) {

        for (int i = 0 ; i < size * typeSize ; ++i)
            buff[i+1] = data[i];

    } else {

        // big endian
        for (int i = 0 ; i < size ; ++i)
            for (int j = 0; j < typeSize; ++j)
                buff[i * typeSize + typeSize - j] = data[i * typeSize + j];

    }

    return transact_(chipAddress, buff, 1 + size * typeSize, nullptr, 0);
}


bool I2C::read_(int chipAddress, int registerAddress,  uint8_t *data, int typeSize, int size, bool litleEndian  ) const
{
    uint8_t buff[size * typeSize];
    buff[0] = registerAddress;

    if (!transact_(chipAddress, buff, 1 , buff, size * typeSize))
        return false;

    if (litleEndian) {

        for (int i = 0; i < size * typeSize; ++i)
            data[i] = buff[i];

    } else {

        //big endian
        for (int i = 0 ; i < size ; ++i)
            for (int j = 0; j < typeSize; ++j)
                data[i * typeSize + j] = buff[i * typeSize + typeSize - j - 1];

    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------
// INSTANTIATION

template bool I2C::writeLe(int chipAddress, int registerAddress, uint8_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, uint16_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, uint32_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, uint64_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, int8_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, int16_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, int32_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, int64_t val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, float val) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, double val) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::writeBe(int chipAddress, int registerAddress, uint8_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, uint16_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, uint32_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, uint64_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, int8_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, int16_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, int32_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, int64_t val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, float val) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, double val) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::writeLe(int chipAddress, int registerAddress, const uint8_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const uint16_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const uint32_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const uint64_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const int8_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const int16_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const int32_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const int64_t *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const float *val, const int count) const;
template bool I2C::writeLe(int chipAddress, int registerAddress, const double *val, const int count) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::writeBe(int chipAddress, int registerAddress, const uint8_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const uint16_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const uint32_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const uint64_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const int8_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const int16_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const int32_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const int64_t *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const float *val, const int count) const;
template bool I2C::writeBe(int chipAddress, int registerAddress, const double *val, const int count) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::readLe(int chipAddress, int registerAddress, uint8_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint16_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint32_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint64_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int8_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int16_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int32_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int64_t* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, float* val) const;
template bool I2C::readLe(int chipAddress, int registerAddress, double* val) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::readBe(int chipAddress, int registerAddress, uint8_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint16_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint32_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint64_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int8_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int16_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int32_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int64_t* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, float* val) const;
template bool I2C::readBe(int chipAddress, int registerAddress, double* val) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::readLe(int chipAddress, int registerAddress, uint8_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint16_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint32_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, uint64_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int8_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int16_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int32_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, int64_t* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, float* array, const int count) const;
template bool I2C::readLe(int chipAddress, int registerAddress, double* array, const int count) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::readBe(int chipAddress, int registerAddress, uint8_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint16_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint32_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, uint64_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int8_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int16_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int32_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, int64_t* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, float* array, const int count) const;
template bool I2C::readBe(int chipAddress, int registerAddress, double* array, const int count) const;

#endif
