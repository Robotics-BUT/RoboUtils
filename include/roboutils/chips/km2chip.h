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

namespace RoboUtils::Chips::Km2 {

    enum class Reg : uint8_t {
        SPEED       =  0x00,  // RW 2u32
        ODOMETRY    =  0x01,  // R- 2u32
        STATUS      =  0x01,  // RW 1u16

        CMDRESET    =  0xE0,
        CMDLOAD     =  0xE1,
        CMDSTORE    =  0xE2,

        CFGMAXSPD       =  0xF0,    // RW 1x int16_t
        CFGTIMEOUT      =  0xF1,    // RW 1x uint16_t
        CFGTIMEOUTPWOFF =  0xF2,    // RW 1x uint16_t
        CFGADDR         =  0xFF,    // RW 1x uint8_t (or 2x uint8_t)
    };

    static inline uint8_t operator+(const Reg reg)
    {
        return static_cast<uint8_t>(reg);
    }

    namespace CFGADDR {
        const Reg Reg = Reg::CFGADDR;

        static constexpr uint8_t ToADDR(const int addr)
        {
            return (addr & 0x7F) << 1;
        }

        static constexpr int FromADDR(const uint8_t reg)
        {
            return (reg & 0x7F) << 1;
        }

        enum : uint8_t {
            ADDR = ToADDR(0x7F),
            BCASTEN = (1 << 0),
        };
    }

}