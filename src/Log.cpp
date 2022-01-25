//
// Created by Matous Hybl on 2018-10-12.
//

#include <cstdio>
#include <sstream>
#include <fstream>
#include <utility>
#include <roboutils/Log.h>
#include <roboutils/utils.h>

using namespace RoboUtils;

    string Log::path = "";
    string Log::address = "";
    COMM::UDP Log::udp = COMM::UDP();

    void Log::setPath(string path) {
        ostringstream buffer;
        buffer << path << "/LBot-" << Log::time() << ".log";
        Log::path = buffer.str();
    }

    void Log::setRemoteTarget(string address) {
        Log::address = std::move(address);

        cout << "Logging address was set_ to: " << Log::address;
    }

    void Log::log(const string& level, const string& message) {
        std::ostringstream buffer;

        buffer << "[" << level << "]: " << Log::time() << " - " << message;

        cout << buffer.str() << endl;

        if (!path.empty()) {
            ofstream file(path, ios_base::app);         // RAII: file will be closed automatically
            file << buffer.str() << endl;
        }

        if (!address.empty() ) {

            if (!udp.bound)
                udp.bind(5555);

            udp.sendStr(address, buffer.str());

        }
    }

    string Log::time() {
        std::string timeString;
        time_t rawTime;
        ::time(&rawTime);
        timeString = ctime(&rawTime);
        // remove newline
        return timeString.substr(0, timeString.size() - 1);
    }

    Log Log::error() {
        return Log("ERROR");
    }

    Log Log::warning() {
        return Log("WARNING");
    }

    Log Log::info() {
        return Log("INFO");
    }

    Log Log::debug() {
        return Log("DEBUG");
    }
