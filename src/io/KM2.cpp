/*
 MIT License

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

#include <roboutils/io/KM2.h>
#include <roboutils/chips/km2chip.h>
#include "roboutils/util/timing.h"

using namespace RoboUtils::IO;
using namespace RoboUtils::Chips;

KM2::KM2(const I2C &aBus, int aChipAddress)
 : bus_(aBus), chipAddress_(aChipAddress)
{
}

KM2::operator bool() const
{
    return bus_;
}

const I2C &KM2::bus() const
{
    return bus_;
}

int KM2::chip() const
{
    return chipAddress_;
}

void KM2::drive(int left, int right) const
{
    int16_t speed[2] {(int16_t)left,(int16_t)right};

    if (!bus_.write(chipAddress_, +Km2::Reg::SPEED, speed, 2, Endian::Little))
        throw km2_error();
}

std::tuple<int,int> KM2::odometry() const
{
    int32_t result[2] {0 , 0};

    if (!bus_.read(chipAddress_, +Km2::Reg::ODOMETRY, result, 2, Endian::Little))
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

    if (!bus_.write(chipAddress_, +Km2::Reg::CFGADDR, addr, Endian::Little))
        throw km2_error();
}