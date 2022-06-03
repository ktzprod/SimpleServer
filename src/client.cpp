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
}
