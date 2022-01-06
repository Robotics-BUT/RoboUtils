/*
 * This file is part of the KAMBot project.
 * 
 *  Copyright (C) 2016 Frantisek Burian <bufran _at_ seznam.cz>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MCP23017_H
#define MCP23017_H

#include <cstdint>

#if defined(__linux__)

namespace RoboUtils::Chips::Mcp23017 {

    static inline int Addr(int id)
    {
        return (0x20 | ((id) & 7));
    }

// bank 0 scheme - the power on reset value
// all registers little endian (A = lo, B= hi)

    enum class Reg : uint8_t {
        IODIR       = 0x00,      // 16bit
        IOPOL       = 0x02,      // 16bit
        GPINTEN     = 0x04,      // 16bit
        DEFVAL      = 0x06,      // 16bit
        INTCON      = 0x08,      // 16bit
        IOCON1      = 0x0A,      // 8bit same as IOCON2
        IOCON2      = 0x0B,      // 8bit same as IOCON1
        GPPU        = 0x0C,      // 16bit
        INTF        = 0x0E,      // 16bit
        INTCAP      = 0x10,      // 16bit
        GPIO        = 0x12,      // 16bit
        OLAT        = 0x14,      // 16bit
    };

    static inline uint8_t operator+(const Reg reg)
    {
        return static_cast<uint8_t>(reg);
    }

    namespace IOCON {
        const Reg Reg = Reg::IOCON1;

        const uint8_t BANK = (1 << 7);
        const uint8_t MIRROR = (1 << 6);
        const uint8_t SEQOP = (1 << 5);
        const uint8_t DISSLW = (1 << 4);
        const uint8_t HAEN = (1 << 3);
        const uint8_t ODR = (1 << 2);
        const uint8_t INTPOL = (1 << 1);
    }

    namespace Pins {
        static const uint16_t PB7 = (1 << 15);
        static const uint16_t PB6 = (1 << 14);
        static const uint16_t PB5 = (1 << 13);
        static const uint16_t PB4 = (1 << 12);
        static const uint16_t PB3 = (1 << 11);
        static const uint16_t PB2 = (1 << 10);
        static const uint16_t PB1 = (1 << 9);
        static const uint16_t PB0 = (1 << 8);
        static const uint16_t PA7 = (1 << 7);
        static const uint16_t PA6 = (1 << 6);
        static const uint16_t PA5 = (1 << 5);
        static const uint16_t PA4 = (1 << 4);
        static const uint16_t PA3 = (1 << 3);
        static const uint16_t PA2 = (1 << 2);
        static const uint16_t PA1 = (1 << 1);
        static const uint16_t PA0 = (1 << 0);
    }
};

#endif

#endif /* MCP23017_H */

