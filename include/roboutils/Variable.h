//
// Created by Matous Hybl on 2018-11-14.
//

#ifndef FIRMWARE_VARIABLE_H
#define FIRMWARE_VARIABLE_H

#include <mutex>

namespace RoboUtils {
    template<typename T>
    class Variable {
    public:
        explicit Variable(T value) {
            this->value = value;
        }

        void set(T value);

        T get();

    private:
        T value;
        std::mutex mutex;
    };

    template<typename T>
    void Variable<T>::set(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        this->value = value;
    }

    template<typename T>
    T Variable<T>::get() {
        std::lock_guard<std::mutex> lock(mutex);
        return value;
    }
}


#endif //FIRMWARE_VARIABLE_H
