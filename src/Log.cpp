//
// Created by Matous Hybl on 2018-10-12.
//

#include <cstdio>
#include <sstream>
#include <fstream>
#include <utility>
#include "roboutils/Log.h"
#include "roboutils/utils.h"

namespace RoboUtils {

    string Log::path = "";
    string Log::address = "";
    uint16_t Log::port = 0;
    COMM::UDP Log::udp = COMM::UDP(5555);

    void Log::setPath(string path) {
        ostringstream buffer;
        buffer << path << "/LBot-" << Log::time() << ".log";
        Log::path = buffer.str();
    }

    void Log::setRemoteTarget(string address, int port) {
        Log::address = std::move(address);
        Log::port = static_cast<uint16_t>(port);

        cout << "Logging address was set to: " << Log::address << ":" << port;
    }

    void Log::log(string level, string message) {
        std::ostringstream buffer;

        buffer << "[" << level << "]: " << Log::time() << " - " << message;

        cout << buffer.str() << endl;

        if (Log::path.empty()) {
            cout << "Log: Logging path was not set." << endl;
            return;
        }

        ofstream file(Log::path, ios_base::app);
        file << buffer.str() << endl;
        file.close();

        if (Log::address.empty() || Log::port == 0) {
            return;
        }

        Log::udp.send(address.c_str(), port, (unsigned char *) buffer.str().c_str(), buffer.str().size());
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

};
