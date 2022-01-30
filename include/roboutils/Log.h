#pragma once
//
// Created by Matous Hybl on 2018-10-12.
//

#include <utility>
#include <iostream>
#include <string>
#include <functional>
#include <sstream>

#include <roboutils/comm/UDP.h>

namespace RoboUtils {

    class Log {
    private:
        const char * level_{nullptr};

        class LogLine : public std::stringstream {
        public:
            explicit LogLine(const Log & parent);
            LogLine(LogLine &line);
            ~LogLine() override;
        };

    public:
        static const Log error;
        static const Log warning;
        static const Log info;
        static const Log debug;

        static std::function<void(const std::string &)> writer;

        explicit constexpr Log(const char *level)
           : level_(level)
        {
        }

        template<class T>
        LogLine operator<<(const T &value) const {
            LogLine line(*this);
            line << value;
            return line;
        }

        LogLine line() const {
            return LogLine(*this);
        }

    };


}
