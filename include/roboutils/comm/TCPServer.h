//
// Created by Matous Hybl on 4/9/19.
//

#ifndef ROBOUTILS_TCPSERVER_H
#define ROBOUTILS_TCPSERVER_H

#include <cstdint>
#include <set>

class TCPServer {
public:
    explicit TCPServer(uint16_t port);

    int acceptConnection();

    void closeConnection(int fd);

    int getFileDescriptor();

    const std::set<int> getOpenedConnections() const;

private:
    int fd;
    std::set<int> openedConnections;
};


#endif //ROBOUTILS_TCPSERVER_H
