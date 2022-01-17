//
// Created by Matous Hybl on 2018-10-19.
//

#ifndef FIRMWARE_UDP_H
#define FIRMWARE_UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>

namespace RoboUtils {
    using std::string;

    class UDP {
    public:
        UDP(uint16_t port);

        ~UDP();

        void receive(unsigned char **buffer);

        void send(const char *host, uint16_t port, unsigned char *buffer, uint32_t size);

        template<typename T>
        void send(string host, uint16_t port, T &data) {
            uint8_t buffer[sizeof(T)];
            size_t size = sizeof(buffer);
            memcpy(buffer, &data, sizeof(T));
            sockaddr_in ca = {};
            memset(&ca, 0, sizeof(ca));
            ca.sin_family = AF_INET;
            ca.sin_addr.s_addr = inet_addr(host.c_str());

            ca.sin_port = htons(port);
            if (sendto(socketDescriptor, buffer, size, 0, (struct sockaddr *) &ca, sizeof(ca)) != size) {
                std::cout << "Failed to send UDP packet to: " << host << ":" << port << std::endl;
            }
        }

        bool available();

    private:
        int socketDescriptor = 0;
    };
};

#endif //FIRMWARE_UDP_H
