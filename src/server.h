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

            /**
             * @brief create a server based on the given arguments
             *
             * @param ip
             * @param port
             *
             * @return a Server instance
             */
            static std::unique_ptr<Server> create(const std::string& ip, const std::string& port);

            // Public API

            /**
             * @brief Check if a new client tries to connect
             *
             * @return a Client instance
             */
            std::unique_ptr<Client> accept_new_client();

            /**
             * @brief Run the server loop
             *
             * @param sleep_ms the threshold at which the server wait between each loop
             *
             * @param end_condition the condition for which the server should stop
             */
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

            /**
             * @brief main run loop logic
             */
            void run_internal();

            /**
             * @brief on client failure print a dedicated message and remove the client from the client list
             */
            ClientContainer::iterator on_client_failure(ClientContainer::iterator& it);

            /**
             * @brief send the given message to all clients
             */
            void send_all(const std::string& msg);

        private:

            ClientContainer clients;
            int server_socket;
    };
}
