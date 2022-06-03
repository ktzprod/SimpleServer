#pragma once

namespace Connectivity {

    class Client
    {
        public:
            Client(int client_socket);
            ~Client();

        public:

            /**
             * @brief Send the given message to the client
             *
             * @param data
             * @param data_size
             *
             * @return false if the client can't be reached, true if the message has been sent properly
             */
            bool send(const void* data, size_t data_size) const;

        private:
            int client_socket;
    };
}
