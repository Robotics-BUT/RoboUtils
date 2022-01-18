//
// Created by Matous Hybl on 2018-10-19.
//

#include <roboutils/comm/UDP.h>

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace RoboUtils::COMM;

UDP::UDP()
{
    // FIXME wrong api - constructor should not throw !
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

UDP::~UDP()
{
    // FIXME - close only if opened
    close(socketDescriptor);
}

void UDP::bind(uint16_t port)
{
    sockaddr_in sa{};
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
    if (::bind(socketDescriptor, (struct sockaddr *) &sa, sizeof(sa)) < 0)
        throw std::logic_error("Failed to bind UDP packet");

    bound = true;
}

std::vector<uint8_t> UDP::receive() const
{
    if (!bound)
        throw std::logic_error("UDP is not bound");

    uint8_t data[1500];

    sockaddr_in ca{};
    memset(&ca, 0, sizeof(ca));
    ca.sin_family = AF_INET;
    unsigned int clientlen = sizeof(ca);

    auto read = recvfrom(socketDescriptor, data, 1500, 0, (struct sockaddr *) &ca, &clientlen);
    if (read < 0)
        throw std::logic_error("Failed to receive UDP packet");


    return {data, data + read };
}

void UDP::send(const std::string &host, uint16_t port, const uint8_t *buffer, std::size_t size) const
{
    if (!bound)
        throw std::logic_error("UDP is not bound");

    sockaddr_in ca{};
    memset(&ca, 0, sizeof(ca));
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = inet_addr(host.c_str());
    ca.sin_port = htons(port);
    if (sendto(socketDescriptor, buffer, size, 0, (struct sockaddr *) &ca, sizeof(ca)) != size)
        throw std::logic_error("Failed to send UDP packet  ");
}

bool UDP::available() const
{
    if (!bound)
        throw std::logic_error("UDP is not bound");

    fd_set rfd;
    timeval tv{};

    FD_ZERO(&rfd);
    FD_SET(socketDescriptor, &rfd);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (select(socketDescriptor + 1, &rfd, nullptr, nullptr, &tv) < 0)
        throw std::logic_error("Failed to select");

    return (FD_ISSET(socketDescriptor, &rfd));
}

std::string UDP::receiveStr() const
{
    if (!available())
        return {};

    auto r = receive();
    return { r.begin(), r.end() };
}