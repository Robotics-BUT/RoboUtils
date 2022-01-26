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

#include <roboutils/io/I2C.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h> // ioctl
#include <fcntl.h> // O_RDWR
#include <unistd.h>  // open, close

#include <vector>

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils::IO;

I2C::I2C(const std::string &busFile)
{
    // FIXME: Wrong design, breaking constructor rule - only memory allocation allowed, no
    i2cDescriptor = open(busFile.c_str(), O_RDWR);
}

I2C::~I2C()
{
    // FIXME: Do not close when wrongly opened
    close(i2cDescriptor);
}

template <typename T>
bool I2C::write(const int chipAddress, const int registerAddress, const T value, bool littleEndian) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(&value), sizeof(T), 1, littleEndian);
}

template <typename T>
bool I2C::write(const int chipAddress, const int registerAddress, const T* array, const int count, bool littleEndian) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(array), sizeof(T), count, littleEndian);
}

template <typename T>
bool I2C::read(const int chipAddress, const int registerAddress, T* value, bool littleEndian) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(value), sizeof(T), 1, littleEndian);
}

template <typename T>
bool I2C::read(const int chipAddress, const int registerAddress, T* array, const int count, bool littleEndian) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(array), sizeof(T), count, littleEndian);
}

template<typename T>
bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, T setBits, T clearBits, T toggleBits, bool littleEncian) const
{
    T value;

    if (!read(chipAddress, registerAddress, &value, littleEncian))
        return false;

    value = ((value & ~clearBits) | setBits) ^ toggleBits;

    return write(chipAddress, registerAddress, value, littleEncian);
}



//======================================================================================================================
// PRIVATE

//#define LOG // TODO log later using matous's engine
bool I2C::transact_(int addr, uint8_t *w, int wn, uint8_t *r, int rn) const
{
    std::lock_guard <std::mutex> lock(mutex);

    if (i2cDescriptor < 0)
        throw i2c_error();

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
        throw i2c_error();

    if (wn > 0) {
        int err = ::write(i2cDescriptor, w, wn);


#ifdef LOG
        printf("(%d written) ", err);
#endif
        if (err != wn)
            return false;
    }

    if (rn > 0) {
        int err = ::read(i2cDescriptor, r, rn);

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
    std::vector<uint8_t> buff(1+size * typeSize);
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

    return transact_(chipAddress, buff.data(), (int)buff.size(), nullptr, 0);
}


bool I2C::read_(int chipAddress, int registerAddress,  uint8_t *data, int typeSize, int size, bool litleEndian  ) const
{
    std::vector<uint8_t> buff(size * typeSize);

    buff[0] = registerAddress;

    if (!transact_(chipAddress, buff.data(), 1 , buff.data(), (int)buff.size()))
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

template bool I2C::write(int chipAddress, int registerAddress, uint8_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint16_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint32_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint64_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, int8_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, int16_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, int32_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, int64_t val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, float val, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, double val, bool littleEndian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::write(int chipAddress, int registerAddress, const uint8_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint16_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint32_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint64_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int8_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int16_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int32_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int64_t *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const float *val, const int count, bool littleEndian) const;
template bool I2C::write(int chipAddress, int registerAddress, const double *val, const int count, bool littleEndian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::read(int chipAddress, int registerAddress, uint8_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint16_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint32_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint64_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int8_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int16_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int32_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int64_t* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, float* val, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, double* val, bool littleEndian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::read(int chipAddress, int registerAddress, uint8_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint16_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint32_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint64_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int8_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int16_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int32_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, int64_t* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, float* array, const int count, bool littleEndian) const;
template bool I2C::read(int chipAddress, int registerAddress, double* array, const int count, bool littleEndian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint8_t setBits, uint8_t clearBits, uint8_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits, uint16_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint32_t setBits, uint32_t clearBits, uint32_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint64_t setBits, uint64_t clearBits, uint64_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int8_t setBits, int8_t clearBits, int8_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int16_t setBits, int16_t clearBits, int16_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int32_t setBits, int32_t clearBits, int32_t toggleBits, bool littleEndian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int64_t setBits, int64_t clearBits, int64_t toggleBits, bool littleEndian) const;
// float a double nedavaji smysl
