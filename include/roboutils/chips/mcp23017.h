#pragma once
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


#include <cstdint>

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

        enum : uint8_t {
            BANK = (1 << 7),
            MIRROR = (1 << 6),
            SEQOP = (1 << 5),
            DISSLW = (1 << 4),
            HAEN = (1 << 3),
            ODR = (1 << 2),
            INTPOL = (1 << 1),
        };
    }

    namespace Pin {

        enum : uint16_t {
            PB7 = (1 << 15),
            PB6 = (1 << 14),
            PB5 = (1 << 13),
            PB4 = (1 << 12),
            PB3 = (1 << 11),
            PB2 = (1 << 10),
            PB1 = (1 << 9),
            PB0 = (1 << 8),
            PA7 = (1 << 7),
            PA6 = (1 << 6),
            PA5 = (1 << 5),
            PA4 = (1 << 4),
            PA3 = (1 << 3),
            PA2 = (1 << 2),
            PA1 = (1 << 1),
            PA0 = (1 << 0),
        };

    }
};



