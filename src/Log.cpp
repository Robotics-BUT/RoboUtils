//
// Created by Matous Hybl on 2018-10-12.
//
#include <roboutils/Log.h>

using namespace RoboUtils;

const Log Log::error = Log("ERROR");
const Log Log::warning = Log("WARNING");
const Log Log::info = Log("INFO");
const Log Log::debug = Log("DEBUG");
std::function<void(const std::string &)> Log::writer{};

// gran time in constructor
Log::LogLine::LogLine(const Log & parent)
  : std::basic_ios<char>{}, std::stringstream{""}
{
    time_t rawTime;
    ::time(&rawTime);

    std::string timeString = ctime(&rawTime);

    // remove newline
    timeString.pop_back();

    *this << "[" << parent.level_ << "]:" << timeString << " - ";
}

Log::LogLine::LogLine(LogLine &line)
  : std::basic_ios<char>{}, std::stringstream{line.str()}
{
    // delete the original
    line.str(std::string());
}


Log::LogLine::~LogLine()
{
    // flush at destructor in the case it has not been flushed
    if (str().empty())
        return;

    auto w = Log::writer;
    if (w)
        w(this->str());
    str(std::string());

}
