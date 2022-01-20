#include <roboutils/strings.h>

#include <sstream>

using namespace RoboUtils;

std::vector<std::string> RoboUtils::split(const std::string& text, char delim) {
    std::string line;
    std::vector<std::string> vec;
    std::stringstream ss(text);

    while (std::getline(ss, line, delim))
        vec.push_back(line);

    return vec;
}

std::string RoboUtils::join(const std::vector<std::string>& data, const std::string& delim)
{
    std::string result;
    std::string d;

    result.reserve(data.size() * 12); // 12 chars per single data on average

    for (auto i : data) {
        result.append(d);
        result.append(i);
        d = delim;
    }

    return result;
}
