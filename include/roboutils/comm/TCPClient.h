//
// Created by Matous Hybl on 4/9/19.
//

#ifndef ROBOUTILS_TCPCLIENT_H
#define ROBOUTILS_TCPCLIENT_H

#include <string>
#include <cstdint>

class TCPClient {
public:
    TCPClient(std::string address, uint16_t port);
    void writeToSocket(uint8_t * data, uint16_t length);

    int getFileDescriptor();

private:
    int fd = 0;
};


#endif //ROBOUTILS_TCPCLIENT_H
