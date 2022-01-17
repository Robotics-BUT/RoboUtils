//
// Created by bufran on 13.01.22.
//

#include "roboutils/io/KM2.h"
#include "roboutils/chips/km2chip.h"
#include "roboutils/utils.h"

#if !defined(__linux__)
# error "This part of library is not compatible with your setup"
#endif

using namespace RoboUtils;
using namespace RoboUtils::Chips;

KM2::KM2(I2C *aBus, int aChipAddress)
{
    bus = aBus;
    chipAddress = aChipAddress;
}

void KM2::drive(int left, int right) const
{
    int16_t speed[2] {(int16_t)left,(int16_t)right};

    if (!bus->write(chipAddress, +Km2::Reg::SPEED, speed, 2, true))
        throw km2_error();
}

std::tuple<int,int> KM2::odometry() const
{
    int32_t result[2] {0 , 0};

    if (!bus->read(chipAddress, +Km2::Reg::ODOMETRY, result, 2, true))
        throw km2_error();

    return std::tie(result[0], result[1]);
}

std::tuple<int,int> KM2::driveodo(int left, int right) const
{
    drive(left,right);
    delay(10); // minimální odstup těchto zpráv aby se ovladač motoru "nezbláznil"
    return odometry();
}

void KM2::setAddress(int newaddr, bool bcast) const
{
    uint8_t addr = Km2::CFGADDR::ToADDR(newaddr);
    if (bcast)
        addr |= Km2::CFGADDR::BCASTEN;

    if (!bus->write(chipAddress, +Km2::Reg::CFGADDR, addr, true))
        throw km2_error();
}