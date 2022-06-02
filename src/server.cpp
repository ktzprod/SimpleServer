#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include "server.h"

namespace Connectivity {

    Server::Server(int sock)
        : server_socket(sock)
    {
        // nothing to do
    }

    Server::~Server()
    {
        close(server_socket);
    }

    std::unique_ptr<Server> Server::create(const std::string& ip, const std::string& port)
    {
        struct addrinfo hints, *p;
        std::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_UNSPEC; // don't constraint to IPv4 or IPv6 only
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags    = AI_PASSIVE;

        int res = getaddrinfo(ip.c_str(), port.c_str(), &hints, &p);
        if (res != 0) {
            std::cerr << gai_strerror(res) << "\n";
            return nullptr;
        }

        if (p == NULL) {
            std::cerr << "No addresses found\n";
            return nullptr;
        }

        // get the socket descriptor
        int server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (res == -1) {
            std::cerr << "Error while creating socket\n";
            freeaddrinfo(p);
            return nullptr;
        }

        auto on_error = [&server_socket, &p](const std::string& error) {
            std::cerr << error << std::endl;
            close(server_socket);
            freeaddrinfo(p);
            return nullptr;
        };

        res = bind(server_socket, p->ai_addr, p->ai_addrlen);
        if (res == -1) {
            return on_error("Error while binding socket");
        }

        res = listen(server_socket, 99);
        if (res == -1) {
            return on_error("Error while binding socket");
        }

        freeaddrinfo(p);
        return std::unique_ptr<Server>(new Server(server_socket));
    }

}
