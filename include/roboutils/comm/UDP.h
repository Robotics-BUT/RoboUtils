#pragma once
/*
 MIT License

Copyright (c) 2019-2021 Matous Hybl
Copyright (c) 2022 Frantisek Burian

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>


namespace RoboUtils::COMM {

    class UDP {
    public:
        UDP(uint16_t port);

        ~UDP();

        void receive(unsigned char **buffer);

        void send(const char *host, uint16_t port, unsigned char *buffer, uint32_t size);

        template<typename T>
        void send(std::string host, uint16_t port, T &data) {
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
}
