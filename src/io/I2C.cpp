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
#include <linux/i2c.h>
#include <fcntl.h> // O_RDWR
#include <unistd.h>  // open, close

#include <vector>

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils::IO;


I2C::~I2C()
{
    if (i2cDescriptor > 0) {
        close(i2cDescriptor);
        i2cDescriptor = 0;
    }
}

I2C::operator bool() const
{
    return i2cDescriptor > 0;
}

bool I2C::open(const std::string &busFile)
{
    if (i2cDescriptor > 0) {
        close(i2cDescriptor);
        i2cDescriptor = 0;
    }

    bus_ = busFile;
    if (bus_.empty())
        return false;

    i2cDescriptor = ::open(bus_.c_str(), O_RDWR);
    return i2cDescriptor > 0;
}

std::string I2C::bus() const
{
    return bus_;
}

template <typename T>
bool I2C::write(const int chipAddress, const int registerAddress, const T value, Endian endian) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(&value), sizeof(T), 1, endian);
}

template <typename T>
bool I2C::write(const int chipAddress, const int registerAddress, const T* array, const int count, Endian endian) const
{
    return write_(chipAddress, registerAddress, reinterpret_cast<const uint8_t *>(array), sizeof(T), count, endian);
}

template <typename T>
bool I2C::read(const int chipAddress, const int registerAddress, T* value, Endian endian) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(value), sizeof(T), 1, endian);
}

template <typename T>
bool I2C::read(const int chipAddress, const int registerAddress, T* array, const int count, Endian endian) const
{
    return read_(chipAddress, registerAddress, reinterpret_cast<uint8_t *>(array), sizeof(T), count, endian);
}

template<typename T>
bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, T setBits, T clearBits, T toggleBits, Endian endian) const
{
    T value;

    if (!read(chipAddress, registerAddress, &value, endian))
        return false;

    value = ((value & ~clearBits) | setBits) ^ toggleBits;

    return write(chipAddress, registerAddress, value, endian);
}



//======================================================================================================================
// PRIVATE

//#define LOG // TODO log later using matous's engine
bool I2C::transact_(int addr, uint8_t *w, int wn, uint8_t *r, int rn) const
{
  std::lock_guard <std::mutex> lock(mutex);

  if (i2cDescriptor < 0)
    throw i2c_error();

  if (wn && rn) {

    i2c_msg messages[2] = {
            {static_cast<uint16_t>(addr), 0,        static_cast<uint16_t>(wn), w},
            {static_cast<uint16_t>(addr), I2C_M_RD, static_cast<uint16_t>(rn), r}
    };

    i2c_rdwr_ioctl_data command = {messages, 2};
    return ::ioctl(i2cDescriptor, I2C_RDWR, &command) > 0;

  } else if (wn) {
    i2c_msg messages[1] = {
            {static_cast<uint16_t>(addr), 0,        static_cast<uint16_t>(wn), w},
    };

    i2c_rdwr_ioctl_data command = {messages, 1};
    return ::ioctl(i2cDescriptor, I2C_RDWR, &command) > 0;

  } else if (rn) {

    i2c_msg messages[1] = {
            {static_cast<uint16_t>(addr), I2C_M_RD, static_cast<uint16_t>(rn), r}
    };

    i2c_rdwr_ioctl_data command = {messages, 1};
    return ::ioctl(i2cDescriptor, I2C_RDWR, &command) > 0;
  }

  return false;
}

bool I2C::write_(int chipAddress, int registerAddress,  const uint8_t *data, int typeSize, int size, Endian endian) const
{
    std::vector<uint8_t> buff(1+size * typeSize);
    buff[0] = registerAddress;

    if (endian == Endian::Little) {

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


bool I2C::read_(int chipAddress, int registerAddress,  uint8_t *data, int typeSize, int size, Endian endian) const
{
    std::vector<uint8_t> buff(size * typeSize);

    buff[0] = registerAddress;

    if (!transact_(chipAddress, buff.data(), 1 , buff.data(), (int)buff.size()))
        return false;

    if (endian == Endian::Little) {

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

template bool I2C::write(int chipAddress, int registerAddress, uint8_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint16_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint32_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, uint64_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, int8_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, int16_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, int32_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, int64_t val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, float val, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, double val, Endian endian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::write(int chipAddress, int registerAddress, const uint8_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint16_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint32_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const uint64_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int8_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int16_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int32_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const int64_t *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const float *val, const int count, Endian endian) const;
template bool I2C::write(int chipAddress, int registerAddress, const double *val, const int count, Endian endian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::read(int chipAddress, int registerAddress, uint8_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint16_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint32_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint64_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int8_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int16_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int32_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int64_t* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, float* val, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, double* val, Endian endian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::read(int chipAddress, int registerAddress, uint8_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint16_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint32_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, uint64_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int8_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int16_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int32_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, int64_t* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, float* array, const int count, Endian endian) const;
template bool I2C::read(int chipAddress, int registerAddress, double* array, const int count, Endian endian) const;

//----------------------------------------------------------------------------------------------------------------------

template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint8_t setBits, uint8_t clearBits, uint8_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits, uint16_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint32_t setBits, uint32_t clearBits, uint32_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, uint64_t setBits, uint64_t clearBits, uint64_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int8_t setBits, int8_t clearBits, int8_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int16_t setBits, int16_t clearBits, int16_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int32_t setBits, int32_t clearBits, int32_t toggleBits, Endian endian) const;
template bool I2C::update(uint8_t chipAddress, uint8_t registerAddress, int64_t setBits, int64_t clearBits, int64_t toggleBits, Endian endian) const;
// float a double nedavaji smysl
