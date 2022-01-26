//
// Created by Matous Hybl on 2019-02-11.
//

#include <thread>
#include "roboutils/threading.h"
#include "roboutils/util/timing.h"
#include "roboutils/util/swift.h"

using namespace RoboUtils;

void repeatAsynchronously(unsigned int periodMs, const std::function<bool(void)>& fun)
{
    std::thread([periodMs, fun]() {
        bool ret = true;
        while (ret) {
            let start = millis();
            ret = fun();
            let delayTime = periodMs - (millis() - start);
            if (delayTime > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(periodMs));
            }
        }
    }).detach();
}