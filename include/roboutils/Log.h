#include <utility>

//
// Created by Matous Hybl on 2018-10-12.
//

#ifndef FIRMWARE_LOG_H
#define FIRMWARE_LOG_H

#include <iostream>
#include <string>
#include "roboutils/comm/UDP.h"

namespace RoboUtils {
    using namespace std;

    class Log {
    public:

        Log(string level) {
            this->level = std::move(level);
        }

        static Log error();

        static Log warning();

        static Log info();

        static Log debug();

        static void setPath(string path);

        static void setRemoteTarget(string address, int port);

        template<class T>
        Log &operator<<(const T &value) {
            buffer << value;

            return *this;
        }

        Log &operator<<(std::ostream &(*manipulator)(std::ostream &)) {
            if (manipulator == static_cast<std::ostream &(*)(std::ostream &)>(std::flush) ||
                manipulator == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {
                Log::log(level, buffer.str());

                buffer.flush();
            }

            return *this;
        }

    private:
        stringstream buffer;
        string level;

        static string path;
        static string address;
        static uint16_t port;
        static COMM::UDP udp;

        static void log(string level, string message);

        static string time();
    };
};


#endif //FIRMWARE_LOG_H
