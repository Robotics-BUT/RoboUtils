//
// Created by Matous Hybl on 4/9/19.
//

#include <roboutils/comm/TCPClient.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace RoboUtils::COMM;

TCPClient::TCPClient(std::string address, uint16_t port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);

    if (connect(fd, (sockaddr *) &addr, sizeof(addr)) != 0) {
        throw -1;
    }
}

void TCPClient::writeToSocket(uint8_t *data, uint16_t length) {
    write(fd, data, length);
}

int TCPClient::getFileDescriptor() {
    return fd;
}
