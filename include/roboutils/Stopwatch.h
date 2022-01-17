#ifndef FIRMWARE_STOPWATCH_H
#define FIRMWARE_STOPWATCH_H

#include <string>
#include <map>

namespace RoboUtils {

    class Stopwatch {
    public:
        Stopwatch(std::string lbl);

        ~Stopwatch();

        long long Trigger(bool print = false);

    private:
        long long Time;
        std::string Label;
    };

}

#endif
