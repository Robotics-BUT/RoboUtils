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
        /// \return true if there is no error during transfer
        template<typename T> bool writeLe(int chipAddress, int registerAddress, T value) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Write register value on the bus as BigEndian
        ///
        /// \note the type of value can be any standard C++ value type from <cstdint>, ie:
        ///  uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double
        ///  the string type is not supported !
        ///
        /// \tparam T type of value to transfer
        /// \param chipAddress the address of chip on bus to be communicated with
        /// \param registerAddress the address of the register on the chip
        /// \param value the value to be written
        /// \return true if there is no error during transfer
        template<typename T> bool writeBe(int chipAddress, int registerAddress, T value) const;

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
        template<typename T> bool writeLe(int chipAddress, int registerAddress, const T *array, int count) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Write an array of register values on the bus as BigEndian
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
        template<typename T> bool writeBe(int chipAddress, int registerAddress, const T *array, int count) const;

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
        template<typename T> bool readLe(int chipAddress, int registerAddress, T *value) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Read register value from the bus as BigEndian
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
        template<typename T> bool readBe(int chipAddress, int registerAddress, T *value) const;

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
        template<typename T> bool readLe(int chipAddress, int registerAddress, T *array, int count) const;

        ///-------------------------------------------------------------------------------------------------------------
        /// \brief Read array of register values from the bus as BigEndian
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
        template<typename T> bool readBe(int chipAddress, int registerAddress, T *array, int count) const;

        void write16bitArray(uint8_t chipAddress, uint8_t registerAddress, int16_t array[], uint8_t arraySize);

        void read32bitArray(uint8_t chipAddress, uint8_t registerAddress, int32_t *array, uint8_t arraySize);

        uint16_t read16bitBEValue(uint8_t chipAddress, uint8_t registerAddress);

        uint16_t read16bitLEValue(uint8_t chipAddress, uint8_t registerAddress);

        void write16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t value);

        void update16bitLEValue(uint8_t chipAddress, uint8_t registerAddress, uint16_t setBits, uint16_t clearBits,
                                uint16_t toggleBits);

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
};

#endif


#endif //FIRMWARE_I2C_H
