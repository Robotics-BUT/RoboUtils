//
// Created by bufran on 13.01.22.
//

#pragma once

#include "roboutils/I2C.h"

#include <tuple>

namespace RoboUtils {

    class KM2 {
    public:
        explicit KM2(I2C *aBus, int aChipAddress = 0x71);

        void drive(int left, int right) const;

        std::tuple<int,int> odometry() const;

        std::tuple<int,int> driveodo(int left, int right) const;

        void setAddress(int newaddr, bool bcast) const;

    private:
        I2C *bus;

        int chipAddress;
    };

    class km2_error : public std::logic_error {
    public:
        km2_error() : std::logic_error("ADC: Access error") {}
    };
};

