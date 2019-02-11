#ifndef FIRMWARE_STOPWATCH_H
#define FIRMWARE_STOPWATCH_H

#include <string>
#include <map>

using std::string;

namespace RoboUtils {
    class Stopwatch {
    public:
        static void start(string label = "default");

        static long long stop(string label = "default");

    private:
        static std::map<string, long long> measurements;
    };
};

#endif
