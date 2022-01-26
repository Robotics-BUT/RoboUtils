#include <utility>

//
// Created by Matous Hybl on 2018-10-12.
//

#ifndef FIRMWARE_LOG_H
#define FIRMWARE_LOG_H

#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include "roboutils/comm/UDP.h"

namespace RoboUtils {

    class Log;

    class LogLine {


        friend class Log;
    public:
        explicit LogLine(const Log * parent);
        LogLine(LogLine &line);
        ~LogLine();

        template<class T>
        LogLine &operator<<(const T &value)  {
            buffer_ << value;
            return *this;
        }

        LogLine &operator<<(std::ostream &(*manipulator)(std::ostream &));

    private:
        std::stringstream buffer_{};
        const Log *parent_{nullptr};
        std::string time_;
    };

    class Log {
        friend class LogLine;

    public:
        // TODO this should be constexpr in the future ...
        static const Log error;
        static const Log warning;
        static const Log info;
        static const Log debug;

        static std::function<void(const std::string &)> writer;

        explicit constexpr Log(const char *level);

        template<class T>
        LogLine operator<<(const T &value) const {
            LogLine line(this);
            line << value;
            return line;
        }

        void log(const std::string& message) const;
        void log(const LogLine& message) const;

    private:
        const char * level_;

        static std::string time();
    };


}


#endif //FIRMWARE_LOG_H
