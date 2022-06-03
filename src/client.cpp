#include <sys/socket.h>
#include <unistd.h>

#include "client.h"

namespace Connectivity {

    Client::Client(int socket)
        : client_socket(socket)
    {
        // nothing to do
    }

    Client::~Client()
    {
        close(client_socket);
    }

    bool Client::send(const void* data, size_t data_size) const
    {
        const char* data_ptr = (const char*)data;
        while (data_size > 0)
        {
            auto bytes_sent = ::send(client_socket, data_ptr, data_size, 0);
            if (bytes_sent < 0) {
                return false;
            }

            data_ptr += bytes_sent;
            data_size -= bytes_sent;
        }

        return true;
    }
}
