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

#ifndef FIRMWARE_I2C_H
#define FIRMWARE_I2C_H

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

#include <mutex>
#include <cstdint>

namespace RoboUtils::IO {
    class I2C {
    public:

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief construct the bus accessor
        ///
        /// \param busFile filename of the linux bus driver
        explicit I2C(const std::string &busFile = "/dev/i2c-1");

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief destructor of the bus accessor
        ///
        /// this destructs all system resources
        ~I2C();

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Write register value on the bus as LittleEndian
        ///
        /// \note the type of value can be any standard C++ value type from <cstdint>, ie:
        ///  uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double
        ///  the string type is not supported !
        ///
        /// \tparam T type of value to transfer
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param value the value to be written
        /// \param littleEndian the endianity of he transaction
        /// \return true if there is no error during transfer
        template<typename T> bool write(int chipAddress, int registerAddress, T value, bool littleEndian=true) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Write an array of register values on the bus as LittleEndian
        ///
        /// \note the type of value can be any standard C++ value type from <cstdint>, ie:
        ///  uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double
        ///  the string type is not supported !
        ///
        /// \tparam T type of value to transfer
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param array the array of values to be written
        /// \param count the array size
        /// \return true if there is no error during transfer
        template<typename T> bool write(int chipAddress, int registerAddress, const T *array, int count, bool littleEndian=true) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Read register value from the bus as LittleEndian
        ///
        /// \note the type of value can be any standard C++ value type from <cstdint>, ie:
        ///  uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double
        ///  the string type is not supported !
        ///
        /// \tparam T type of value to transfer
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param value the value will be filled wirh read data
        /// \return true if there is no error during transfer
        template<typename T> bool read(int chipAddress, int registerAddress, T *value, bool littleEndian=true) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Read array of register values from the bus as LittleEndian
        ///
        /// \note the type of value can be any standard C++ value type from <cstdint>, ie:
        ///  uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double
        ///  the string type is not supported !
        ///
        /// \tparam T type of value to transfer
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param array the array of values to be filled with returned data
        /// \param count the array size
        /// \return true if there is no error during transfer
        template<typename T> bool read(int chipAddress, int registerAddress, T *array, int count, bool littleEndian=true) const;


        template<typename T>
        bool update(uint8_t chipAddress, uint8_t registerAddress, T setBits, T clearBits, T toggleBits, bool littleEndian=true) const;

    private:
        /// the file descriptor
        int i2cDescriptor;

        /// the thread-guarding mutex
        mutable std::mutex mutex;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief
        ///
        /// \param addr
        /// \param w
        /// \param wn
        /// \param r
        /// \param rn
        /// \return true if there is no error during transfer
        bool transact_(int addr, uint8_t *w, uint32_t wn, uint8_t *r, uint32_t rn) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief
        ///
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param data the pointer to array of values to be written
        /// \param size the size of each item in the array
        /// \param count the array size
        /// \param littleEndian the endianity of he transaction
        /// \return true if there is no error during transfer
        bool write_(int chipAddress, int registerAddress,  const uint8_t *data, int size, int count, bool litleEndian) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief
        ///
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param data the pointer to array of values to be filled with data
        /// \param size the size of each item in the array
        /// \param count the array size
        /// \param littleEndian the endianity of he transaction
        /// \return true if there is no error during transfer
        bool read_(int chipAddress, int registerAddress,  uint8_t *data, int size, int count, bool litleEndian) const;
    };

    class i2c_error : public std::logic_error {
    public:
        i2c_error() : std::logic_error("I2C: Unrecoverable error") {}
    };
};

#endif