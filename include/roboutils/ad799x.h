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

namespace RoboUtils {
#ifdef __cplusplus
    extern "C" {
#endif


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


// only read  
#define AD799X_RESULT         (0x00)                        // 16bit
// start conversion and read result on channel
#define AD799X_RESULT_CH(ch)  (0x80 | (ch) << 4)            // 16bit
// start sequential conversion and read first result
#define AD799X_RESULT_SEQ     (0x70)                        // 16bit

#define AD799X_ALERT          0x01                          // 8 bit
#define AD799X_CONFIG         0x02                          // 8 bit (C: 16 bit)
#define AD799X_CYCLE          0x03                          // 8 bit
#define AD799X_DATAL(ch)      (0x04 + (ch)*3)               // 16 bit
#define AD799X_DATAH(ch)      (0x05 + (ch)*3)               // 16 bit
#define AD799X_HYST(ch)       (0x06 + (ch)*3)               // 16 bit
#define AD799X_DATAL0         0x04
#define AD799X_DATAH0         0x05
#define AD799X_HYST0          0x06
#define AD799X_DATAL1         0x07
#define AD799X_DATAH1         0x08
#define AD799X_HYST1          0x09
#define AD799X_DATAL2         0x0A
#define AD799X_DATAH2         0x0B
#define AD799X_HYST2          0x0C
#define AD799X_DATAL3         0x0D
#define AD799X_DATAH3         0x0E
#define AD799X_HYST3          0x0F

/*******************************************************************************
 * BITS DESCRIPTION
 */

// ---- AD799X_RESULT ----------------------------------------------------------
#define AD799X_RESULT_ALERT                (1 << 15)
#define AD799X_RESULT_CHAN                 (7 << 12)
#define AD799X_RESULT_CHAN_CH(ch)          ((ch) << 12)
#define AD799X_RESULT_CHAN_CH0             (0 << 12)
#define AD799X_RESULT_CHAN_CH1             (1 << 12)
#define AD799X_RESULT_CHAN_CH2             (2 << 12)
#define AD799X_RESULT_CHAN_CH3             (3 << 12)
#define AD799X_RESULT_CHAN_CH4             (4 << 12)
#define AD799X_RESULT_CHAN_CH5             (5 << 12)
#define AD799X_RESULT_CHAN_CH6             (6 << 12)
#define AD799X_RESULT_CHAN_CH7             (7 << 12)
#define AD799X_RESULT_VALUE                (0x0FFF << 0)

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


#ifdef __cplusplus
    }
#endif
};

#endif
#endif /* AD799X_H */
