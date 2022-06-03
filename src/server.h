#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "client.h"

namespace Connectivity {

    class Server
    {
        public:

            virtual ~Server();

        private:

            using ClientContainer = std::vector<std::unique_ptr<Client>>;
            Server(int socket);

        public:

            // Factory
            static std::unique_ptr<Server> create(const std::string& ip, const std::string& port);

            // Public API
            std::unique_ptr<Client> accept_new_client();

            template<typename Predicate>
            void run(const std::chrono::milliseconds& sleep_ms, Predicate&& end_condition)
            {
                while (!end_condition()) {
                    run_internal();
                    std::this_thread::sleep_for(sleep_ms);
                }

                send_all("bye\n");
                clients.clear();
            }

        private:

            void run_internal();
            ClientContainer::iterator on_client_failure(ClientContainer::iterator& it);
            void send_all(const std::string& msg);

        private:

            ClientContainer clients;
            int server_socket;
    };
}
