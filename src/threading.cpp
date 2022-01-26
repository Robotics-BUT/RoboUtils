//
// Created by Matous Hybl on 2019-02-11.
//

#include <thread>
#include "roboutils/threading.h"
#include "roboutils/util/timing.h"
#include "roboutils/util/swift.h"

namespace RoboUtils {
    void repeatAsynchronously(unsigned int periodMs, const std::function<void(void)>& fun) {
        std::thread([periodMs, fun]() {
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmissing-noreturn"
            while (true) {
                let start = millis();
                fun();
                let delayTime = periodMs - (millis() - start);
                if (delayTime > 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(periodMs));
                }
            }
//#pragma clang diagnostic pop
        }).detach();
    }
}