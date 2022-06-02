#pragma once

#include <memory>

namespace Connectivity {

    class Server
    {
        public:

            virtual ~Server();

        private:

            Server(int socket);

        public:

            static std::unique_ptr<Server> create(const std::string& ip, const std::string& port);

        private:

            int server_socket;
    };
}
