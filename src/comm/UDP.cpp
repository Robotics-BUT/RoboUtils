//
// Created by Matous Hybl on 2018-10-19.
//

#include <roboutils/comm/UDP.h>

#include <iostream>


using namespace RoboUtils::COMM;


UDP::UDP(uint16_t port) {
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in sa = {};
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
    bind(socketDescriptor, (struct sockaddr *) &sa, sizeof(sa));
}

UDP::~UDP() {
    close(socketDescriptor);
}

void UDP::receive(unsigned char **buffer) {
    unsigned char buf[1500];
    sockaddr_in ca = {};
    memset(&ca, 0, sizeof(ca));
    ca.sin_family = AF_INET;
    unsigned int clientlen = sizeof(ca);
    if (recvfrom(socketDescriptor, buf, 1500, 0, (struct sockaddr *) &ca, &clientlen) == -1) {
        std::cout << "Failed to receive UDP packet" << std::endl;
    }

    *buffer = buf;
}

void UDP::send(const char *host, uint16_t port, uint8_t *buffer, uint32_t size) {
    sockaddr_in ca = {};
    memset(&ca, 0, sizeof(ca));
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = inet_addr(host);
    ca.sin_port = htons(port);
    if (sendto(socketDescriptor, buffer, size, 0, (struct sockaddr *) &ca, sizeof(ca)) != size) {
        std::cout << "Failed to send UDP packet to: " << host << ":" << port << std::endl;
    }
}

bool UDP::available() {
    fd_set rfd;
    timeval tv = {};
    FD_ZERO(&rfd);
    FD_SET(socketDescriptor, &rfd);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (select(socketDescriptor + 1, &rfd, nullptr, nullptr, &tv) < 0) {
        // handle OS error
        return false;
    }

    if (FD_ISSET(socketDescriptor, &rfd)) {
        return true;
    }

    return false;
}

