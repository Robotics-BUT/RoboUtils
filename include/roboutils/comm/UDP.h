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


#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

namespace RoboUtils::COMM {

    class UDP {
    public:
        /// \brief Create the UDP object
        UDP();

        /// \brief Destroy the UDP object
        ~UDP();

        /// \brief Bind to specified UDP port
        ///
        /// \param port the port to bind to (default is automatic)
        void bind(uint16_t port = 0);

        /// \brief Send data to specified host
        ///
        /// \param host the host identification ip/port
        /// \param buffer the data to send
        /// \param size size of data to send
        void send(const std::string &host, const uint8_t *buffer, std::size_t size) const;

        /// \brief Send text data to specified host
        ///
        /// \param host the host identification ip/port
        /// \param data text to be sent
        void sendStr(const std::string &host, const std::string &data) const
        {
            send(host, reinterpret_cast<const uint8_t*>(data.c_str()), data.length());
        }

        /// \brief Return if any packet is waiting for receive
        ///
        /// \return true, if there is waiting packet to receive
        bool available() const;

        /// \brief Receive data from remote
        ///
        /// \return auto [remote, data]
        std::tuple<std::string, std::vector<uint8_t>> receive() const;

        /// \brief Receive text from remote
        ///
        /// \return auto [remote, text]
        std::tuple<std::string, std::string> receiveStr() const;

        bool bound{false};

    private:
        int socketDescriptor = 0;
    };
}
