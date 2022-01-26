//
// Created by Matous Hybl on 2018-10-19.
//

#include <roboutils/comm/UDP.h>
#include <roboutils/strings.h>

#include <algorithm>

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace RoboUtils::COMM;

class SAHelper {
public:
    sockaddr_in sa{};

    SAHelper(uint16_t port = 0)
    {
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = htonl(INADDR_ANY);
        sa.sin_port = htons(port);
    }

    SAHelper(const std::string& host)
     : SAHelper(0)
    {
        auto spl = RoboUtils::split(host, '/');

        if (spl.size() != 2)
            throw std::logic_error("UDP::HOST must have port specified <ip>/<port>");

        if (!std::all_of(spl[1].begin(), spl[1].end(), ::isdigit))
            throw std::logic_error("UDP::HOST port must be only digits <ip>/<port>");

        if (!inet_pton(AF_INET, spl[0].c_str(), &(sa.sin_addr)))
            throw std::logic_error("UDP::HOST ip address is not known");

        sa.sin_port = htons(std::stoi(spl[1]));
    }

    operator std::string()
    {
        char str[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

        std::vector<std::string> sender { str, std::to_string(ntohs(sa.sin_port)) };

        return RoboUtils::join(sender,"/");
    }

};

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
    SAHelper sah(port);

    if (::bind(socketDescriptor, (struct sockaddr *) &sah.sa, sizeof(sah.sa)) < 0)
        throw std::logic_error("Failed to bind UDP packet");

    bound = true;
}

void UDP::send(const std::string &host, const uint8_t *buffer, std::size_t size) const
{
    if (!bound)
        throw std::logic_error("UDP is not bound");

    SAHelper sa(host);

    if (sendto(socketDescriptor, buffer, size, 0, (struct sockaddr *) &sa.sa, sizeof(sa.sa)) != (ssize_t) size)
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
    int cnt = select(socketDescriptor + 1, &rfd, nullptr, nullptr, &tv);
    if (cnt < 0)
        throw std::logic_error("Failed to select");

    if (cnt == 0)
        return false;

    return (FD_ISSET(socketDescriptor, &rfd));
}

std::tuple<std::string, std::vector<uint8_t>> UDP::receive() const
{
    if (!bound)
        throw std::logic_error("UDP is not bound");

    SAHelper sa;
    unsigned int clientlen = sizeof(sa.sa);
    std::vector<uint8_t> result(1500);

    auto read = recvfrom(socketDescriptor, result.data(), result.size(), 0, (struct sockaddr *) &sa.sa, &clientlen);
    if (read < 0)
        throw std::logic_error("Failed to receive UDP packet");

    result.resize(read);

    return std::tie(sa, result);
}

std::tuple<std::string, std::string> UDP::receiveStr() const
{
    if (!available())
        return std::tie("","");

    auto [ sender, r ] = receive();

    std::string val { r.begin(), r.end() };
    return std::tie(sender, val );
}