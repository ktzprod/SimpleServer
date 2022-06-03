#pragma once

namespace Connectivity {

    class Client
    {
        public:
            Client(int client_socket);
            ~Client();

        private:
            int client_socket;
    };
}
