//
// Created by bufran on 13.01.22.
//

#pragma once

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