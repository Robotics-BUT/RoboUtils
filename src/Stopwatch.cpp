#include <iostream>
#include "roboutils/Stopwatch.h"
#include "roboutils/utils.h"

using namespace RoboUtils;
using namespace std;

map<string, long long> Stopwatch::measurements = map<string, long long>();

void Stopwatch::start(const string& label)
{
    measurements[label] = millis();
}

long long Stopwatch::stop(const string& label)
{
    auto stop = millis();
    auto start = measurements[label];

    if (start == 0) {
        measurements[label] = stop;

        std::cout << "Stopwatch: stop called before start." << std::endl;
    }

    const auto diff = stop - measurements[label];
    measurements[label] = stop;

    std::cout << "Measured time for label: " << label << " is: " << diff << " ms" << std::endl;

    return diff;
}

