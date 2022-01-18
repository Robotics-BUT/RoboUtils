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
        UDP();

        ~UDP();

        void bind(uint16_t port);

        void send(const std::string &host, const uint8_t *buffer, std::size_t size) const;

        void sendStr(const std::string &host, const std::string &data) const
        {
            send(host, reinterpret_cast<const uint8_t*>(data.c_str()), data.length());
        }

        bool available() const;

        std::tuple<std::string, std::vector<uint8_t>> receive() const;

        std::tuple<std::string, std::string> receiveStr() const;

        bool bound{false};

    private:
        int socketDescriptor = 0;
    };
}
