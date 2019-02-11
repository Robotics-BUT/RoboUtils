#include <iostream>
#include "roboutils/Stopwatch.h"
#include "roboutils/utils.h"

namespace RoboUtils {

    using namespace std;

    map<string, long long> Stopwatch::measurements = map<string, long long>();

    void Stopwatch::start(string label) {
        measurements[label] = millis();
    }

    long long Stopwatch::stop(string label) {
        auto stop = millis();
        auto start = measurements[label];

        if (start == 0) {
            measurements[label] = stop;

            cout << "Stopwatch: stop called before start." << endl;
        }

        let diff = stop - measurements[label];
        measurements[label] = stop;

        cout << "Measured time for label: " << label << " is: " << diff << " ms" << endl;

        return diff;
    }
};
