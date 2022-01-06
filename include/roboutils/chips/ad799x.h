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
#ifndef AD799X_H
#define AD799X_H

#if defined(__linux__)

namespace RoboUtils::Chips::Ad799X {




// versions
// chip     channel  bits  group
// AD7991   4channel 12bit  B
// AD7992   2channel 12bit    D
// AD7993   4channel 10bit A
// AD7994   4channel 12bit A
// AD7995   4channel 10bit  B
// AD7996   -- not exist
// AD7997   8channel 10bit   C
// AD7998   8channel 12bit   C
// AD7999   4channel 08bit  B

// groups
// A - base chips
// B - these does not have addresses at all, CONFIG register is first byte 
//     written, RESULT is first word readed
// C - these have longer CONFIG register 16 bits wide
// D -   

// I2C addresses: <chip>_<version (0 or 1)>_<as pin (low,high or float)>
#define ADDR_AD799X_0_F   0x20
#define ADDR_AD799X_1_F   0x20
#define ADDR_AD799X_0_L   0x21
#define ADDR_AD799X_0_H   0x22
#define ADDR_AD799X_1_L   0x23
#define ADDR_AD799X_1_H   0x24

#define ADDR_AD7991_0     0x28
#define ADDR_AD7991_1     0x29
#define ADDR_AD7995_0     0x28
#define ADDR_AD7995_1     0x29
#define ADDR_AD7999_1     0x29

/*******************************************************************************
 * REGISTERS DESCRIPTION
 */
// Address pointer

    namespace RESULT {
        static constexpr uint8_t Reg(const int channel) {
            return static_cast<uint8_t>(0x80 | (channel << 4));
        }
    }

    namespace DATA {
        static constexpr uint8_t Reg(const int channel)
        {
            return static_cast<uint8_t>(0x04 + channel * 3);
        }
    }

    namespace HYST {
        static constexpr uint8_t Reg(const int channel)
        {
            return static_cast<uint8_t>(0x06 + channel * 3);
        }
    }

    enum class Reg : uint8_t {
        RESULT       =  0x00,                          // 16bit        // only read
        ALERT        =  0x01,                          // 8 bit
        CONFIG       =  0x02,                          // 8 bit (C: 16 bit)
        CYCLE        =  0x03,                          // 8 bit
        DATAL0       =  DATA::Reg(0) + 0,
        DATAH0       =  DATA::Reg(0) + 1,
        HYST0        =  HYST::Reg(0),
        DATAL1       =  DATA::Reg(1) + 0,
        DATAH1       =  DATA::Reg(1) + 1,
        HYST1        =  HYST::Reg(1),
        DATAL2       =  DATA::Reg(2) + 0,
        DATAH2       =  DATA::Reg(2) + 1,
        HYST2        =  HYST::Reg(2),
        DATAL3       =  DATA::Reg(3) + 0,
        DATAH3       =  DATA::Reg(3) + 1,
        HYST3        =  HYST::Reg(3),

        // start sequential conversion and read first result
        RESULT_SEQ   =  0x70,                        // 16bit

        // start conversion and read result on channel
        RESULT0      = RESULT::Reg(0),            // 16bit
        RESULT1      = RESULT::Reg(1),            // 16bit
        RESULT2      = RESULT::Reg(2),            // 16bit
        RESULT3      = RESULT::Reg(3),            // 16bit
    };

    static inline uint8_t operator+(const Reg reg)
    {
        return static_cast<uint8_t>(reg);
    }



/*******************************************************************************
 * BITS DESCRIPTION
 */
    namespace RESULT {

        static constexpr uint16_t ToCHAN(int ch) {
            return ch << 12;
        }

        static constexpr int FromCHAN(uint16_t reg) {
            return (reg >> 12) & 7;
        }

        static constexpr uint16_t ToVALUE(int ch) {
            return ch & 0xFFF;
        }

        static constexpr int FromVALUE(uint16_t reg) {
            return reg & 0xFFF;
        }

        static const uint16_t ALERT       =  1 << 15;

        static const uint16_t CHAN        =  ToCHAN(7 );
        static const uint16_t CHAN_0      =  ToCHAN(0 );
        static const uint16_t CHAN_1      =  ToCHAN(1 );
        static const uint16_t CHAN_2      =  ToCHAN(2 );
        static const uint16_t CHAN_3      =  ToCHAN(3 );
        static const uint16_t CHAN_4      =  ToCHAN(4 );
        static const uint16_t CHAN_5      =  ToCHAN(5 );
        static const uint16_t CHAN_6      =  ToCHAN(6 );
        static const uint16_t CHAN_7      =  ToCHAN(7 );

        static const uint16_t VALUE       =  ToVALUE(0x0FFF);
    }



// ---- AD799X_ALERT -----------------------------------------------------------
#define AD799X_ALERT_L(ch)                 (1 << ((ch) * 2))
#define AD799X_ALERT_H(ch)                 (2 << ((ch) * 2))
#define AD799X_ALERT_L0                    (1 << 0)
#define AD799X_ALERT_H0                    (1 << 1)
#define AD799X_ALERT_L1                    (1 << 2)
#define AD799X_ALERT_H1                    (1 << 3)
#define AD799X_ALERT_L2                    (1 << 4)
#define AD799X_ALERT_H2                    (1 << 5)
#define AD799X_ALERT_L3                    (1 << 6)
#define AD799X_ALERT_H3                    (1 << 7)

// ---- AD799X_CONFIG ----------------------------------------------------------
// generic for group A, C, D*
#define AD799X_CONFIG_CH(ch)               (1 << ((ch)+4))
#define AD799X_CONFIG_CH_ALL               (0xFF << 4)
#define AD799X_CONFIG_FLTR                 (1 << 3)
#define AD799X_CONFIG_ALERTPIN             (3 << 1)
#define AD799X_CONFIG_ALERTPIN_DISABLED    (0 << 1)
#define AD799X_CONFIG_ALERTPIN_BUSY        (1 << 1)
#define AD799X_CONFIG_ALERTPIN_ALERT       (2 << 1)
#define AD799X_CONFIG_ALERTPIN_RESET       (3 << 1)
#define AD799X_CONFIG_ALERTPOL             (1 << 0)
#define AD799X_CONFIG_ALERTPOL_LOW         (0 << 0)
#define AD799X_CONFIG_ALERTPOL_HIGH        (1 << 0)

// group B only
#define AD7991_CONFIG_CH(ch)               (1 << ((ch)+4))
#define AD7991_CONFIG_REFSEL               (1 << 3)
#define AD7991_CONFIG_FLTR                 (1 << 2)
#define AD7991_CONFIG_SAMPDELAY            (1 << 1)
#define AD7991_CONFIG_TRIALDELAY           (1 << 0)

// group D only
#define AD7992_CONFIG_SINGLE               (1 << 6)
// bits 0 - 5 in group D are generic (X)

// ---- AD799X_CYCLE -----------------------------------------------------------
// T is 2us
#define AD799X_CYCLE_SAMPDELAY             (1 << 7)
#define AD799X_CYCLE_TRIALDELAY            (1 << 6)
#define AD799X_CYCLE_CYC                   (7 << 0)
#define AD799X_CYCLE_CYC_NONE              (0 << 0)
#define AD799X_CYCLE_CYC_32T               (1 << 0)
#define AD799X_CYCLE_CYC_64T               (2 << 0)
#define AD799X_CYCLE_CYC_128T              (3 << 0)
#define AD799X_CYCLE_CYC_256T              (4 << 0)
#define AD799X_CYCLE_CYC_512T              (5 << 0)
#define AD799X_CYCLE_CYC_1024T             (6 << 0)
#define AD799X_CYCLE_CYC_2048T             (7 << 0)

// ---- AD799X_DATAL -----------------------------------------------------------
#define AD799X_DATAL_VALUE                 (0x0FFF << 0)

// ---- AD799X_DATAH -----------------------------------------------------------
#define AD799X_DATAH_VALUE                 (0x0FFF << 0)

// ---- AD799X_HYST ------------------------------------------------------------
#define AD799X_HYST_VALUE                  (0x0FFF << 0)


};

#endif
#endif /* AD799X_H */
