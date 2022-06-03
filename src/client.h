#pragma once

namespace Connectivity {

    class Client
    {
        public:
            Client(int client_socket);
            ~Client();

        public:

            bool send(const void* data, size_t data_size) const;

        private:
            int client_socket;
    };
}
