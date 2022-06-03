#pragma once

#include <memory>

#include "client.h"

namespace Connectivity {

    class Server
    {
        public:

            virtual ~Server();

        private:

            Server(int socket);

        public:

            // Factory
            static std::unique_ptr<Server> create(const std::string& ip, const std::string& port);

            // Public API
            std::unique_ptr<Client> accept_new_client();

        private:

            int server_socket;
    };
}
