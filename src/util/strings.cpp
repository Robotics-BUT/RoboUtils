#include "roboutils/util/strings.h"

#include <sstream>
#include <cmath>
#include <iomanip>

using namespace RoboUtils;

std::vector<std::string> RoboUtils::split(const std::string& text, char delim)
{
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

    for (const auto &i : data) {
        result.append(d);
        result.append(i);
        d = delim;
    }

    return result;
}

std::string RoboUtils::join(const std::vector<std::string>::iterator & begin, const std::vector<std::string>::iterator & end, const std::string& delim)
{
  std::vector<std::string>::iterator i = begin;

  if (i == end)
    return {};

  std::string result{*i++};

  while (i != end) {
    result.append(delim);
    result.append(*i++);
  }

  return result;
}

std::string RoboUtils::to_fixed(double d, int decimals)
{
  if (std::isnan(d))
    return {};

  std::ostringstream results;
  results << std::fixed << std::setprecision(decimals) << d;
  return results.str();
}