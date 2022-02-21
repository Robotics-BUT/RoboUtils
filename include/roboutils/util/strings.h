#pragma once

#include <string>
#include <vector>

namespace RoboUtils {

    /// \brief Split the supplied string by delimiter
    ///
    /// \code
    ///   std::string data{"AHOJ:JOHA:OUHA:HAHA:NOHA"};
    ///   auto result = RoboUtils::split(data, ':');    //  == { "AHOJ", "JOHA", "OUHA", "HAHA", "NOHA"}
    /// \endcode
    ///
    /// \param text string to be splitted
    /// \param delim the delimiter
    /// \return splitted vector of string
    std::vector<std::string> split(const std::string& text, char delim);

    /// \brief Join the supplied set of strings with delimiter
    ///
    /// \code
    ///   std::vector<std::string> data{ "AHOJ", "JOHA", "OUHA", "HAHA", "NOHA"};
    ///   auto result = RoboUtils::join(data, ":");    //  == "AHOJ:JOHA:OUHA:HAHA:NOHA
    /// \endcode
    ///
    /// \param data the list of strings
    /// \param delim the delimiter
    /// \return joined vector of string
    std::string join(const std::vector<std::string>& data, const std::string& delim);

    std::string join(const std::vector<std::string>::iterator & begin, const std::vector<std::string>::iterator & end, const std::string& delim);

    std::string to_fixed(double d, int decimals);

    void eraseBefore(std::string &s, char c);
    void eraseBefore(std::string &s, const std::string &c);

}