//
// Created by Matous Hybl on 2018-11-14.
//

#pragma once

#include <mutex>

namespace RoboUtils {

    template<typename T>
    class Variable {
    public:
        explicit Variable(T val) {
            value = val;
        }

        void set(T val) {
            std::lock_guard<std::mutex> lock(mutex);
            value = val;
        }

        T get() {
            std::lock_guard<std::mutex> lock(mutex);
            return value;
        }

    private:
        T value;
        std::mutex mutex;
    };

}



