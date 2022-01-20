#pragma once

#include <string>
#include <vector>

namespace RoboUtils {

    std::vector<std::string> split(const std::string& text, char delim);

    std::string join(const std::vector<std::string>& data, const std::string& delim);

}