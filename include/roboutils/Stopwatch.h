#ifndef FIRMWARE_STOPWATCH_H
#define FIRMWARE_STOPWATCH_H

#include <string>
#include <map>

namespace RoboUtils {

    class Stopwatch {
    public:
        static void start(const std::string& label = "default");

        static long long stop(const std::string& label = "default");

    private:
        static std::map<std::string, long long> measurements;
    };

}

#endif
