//
// Created by Matous Hybl on 2018-10-12.
//

#include <sstream>
#include <roboutils/Log.h>

using namespace RoboUtils;


const Log Log::error = Log("ERROR");
const Log Log::warning = Log("WARNING");
const Log Log::info = Log("INFO");
const Log Log::debug = Log("DEBUG");
std::function<void(const std::string &)> Log::writer{};

constexpr Log::Log(const char *lvl)
 : level_(lvl)
{
}

void Log::log(const std::string& message) const
{
    if (writer)
        writer("[" + std::string(level_) + "]: " + Log::time() + " - " + message);
}

void Log::log(const LogLine& message) const
{
    if (writer)
        writer("[" + std::string(level_) + "]: " + message.time_ + " - " + message.buffer_.str());
}

std::string Log::time()
{
    time_t rawTime;
    ::time(&rawTime);

    std::string timeString = ctime(&rawTime);

    // remove newline
    return timeString.substr(0, timeString.size() - 1);
}

// gran time in constructor
LogLine::LogLine(const Log * parent)
  : parent_(parent) , time_(Log::time())
{
}

LogLine::LogLine(LogLine &line)
  : parent_(line.parent_), time_(line.time_)
{
    buffer_.str(line.buffer_.str());
    line.buffer_.str(std::string());
}


LogLine::~LogLine()
{
    // flush at destructor in the case it has not been flushed
    if (!buffer_.str().empty()) {
        parent_->log(*this);
        buffer_.str(std::string());
    }
}

// flush on endl or flush
LogLine &LogLine::operator<<(std::ostream &(*manipulator)(std::ostream &))
{
    if (manipulator == static_cast<std::ostream &(*)(std::ostream &)>(std::flush) ||
        manipulator == static_cast<std::ostream &(*)(std::ostream &)>(std::endl)) {

        parent_->log(*this);
        buffer_.str(std::string());
    }

    return *this;
}

