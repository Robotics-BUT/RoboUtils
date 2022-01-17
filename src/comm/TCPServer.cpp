//
// Created by Matous Hybl on 4/9/19.
//

#include <roboutils/comm/TCPServer.h>

#include <netinet/in.h>
#include <iostream>
#include <unistd.h>


using namespace RoboUtils::COMM;

TCPServer::TCPServer(uint16_t port) {
    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) {
        std::cout << "Failed to open socket." << std::endl;
        throw -1;
    }

    if (bind(fd, (sockaddr *) &address, sizeof(address)) != 0) {
        std::cout << "Failed to bind socket." << std::endl;
        throw -1;
    }

    if (listen(fd, 5) != 0) {
        std::cout << "Failed to listen socket." << std::endl;
        throw -1;
    }
}

int TCPServer::acceptConnection() {
    sockaddr_in addr = {};
    unsigned int len = sizeof(addr);

    int connectionFd = accept(fd, (sockaddr *) &addr, &len);

    if (connectionFd <= 0) {
        std::cout << "connection failed" << std::endl;
        return -1;
    }

    openedConnections.insert(connectionFd);

    return connectionFd;
}

void TCPServer::closeConnection(int fd) {
    openedConnections.erase(fd);
    close(fd);
}

int TCPServer::getFileDescriptor() {
    return fd;
}

const std::set<int> TCPServer::getOpenedConnections() const {
    return openedConnections;
}

