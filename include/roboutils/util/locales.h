#pragma once

#include <string>

namespace RoboUtils {

    template <int LC>
    class Locale {
    public:
        explicit Locale(const char *loc = "C");

        ~Locale();

    private:
        const std::string loc_;
    };


    using NumericLocale = Locale<LC_NUMERIC>;
    using TimeLocale = Locale<LC_TIME>;
}
