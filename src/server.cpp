#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
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
            std::cerr << "message: " << error << " error: " << strerror(errno) << std::endl;
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

        int flags = fcntl(server_socket, F_GETFL, 0);
        if (flags == -1)
        {
            return on_error("Error while probing flags for socket");
        }

        // non-blocking will allow us to not block when accepting clients
        flags |= O_NONBLOCK;

        res = fcntl(server_socket, F_SETFL, flags);
        if (res == -1)
        {
            return on_error("Failed to modify socket to be non-blocking");
        }

        freeaddrinfo(p);
        return std::unique_ptr<Server>(new Server(server_socket));
    }

    std::unique_ptr<Client> Server::accept_new_client()
    {
        sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_addr_size);
        if (client_socket == -1) {
            if (errno != EWOULDBLOCK) {
                std::cerr << "Error while accepting new client: " << strerror(errno) << std::endl;
            }
            return nullptr;
        }

        return std::unique_ptr<Client>(new Client(client_socket));
    }
}
