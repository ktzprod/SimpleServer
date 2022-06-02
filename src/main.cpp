#include <iostream>

#include "server.h"

#define INVALID_ARGUMENT -1
#define MISSING_ARGUMENT -2

/**
 * @brief print how to use this program
 */
void print_help()
{
    std::cout << "This program must be used as follow:" << std::endl;
    std::cout << "\t" << "test-timeserver ${LISTENING_IP} ${LISTENING_PORT}" << std::endl << std::endl;
    std::cout << "Known errors:" << std::endl;
    std::cout << "\t" << "INVALID_ARGUMENT(" << INVALID_ARGUMENT << ")" << std::endl;
    std::cout << "\t" << "MISSING_ARGUMENT(" << MISSING_ARGUMENT << ")" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief print the arguments of this program
 */
void print_args(const std::string& listening_ip, const std::string& listening_port)
{
    std::cout << "arguments: " << std::endl;
    std::cout << "\t" << "listening_ip: " << listening_ip << std::endl;
    std::cout << "\t" << "listening_port: " << listening_port << std::endl;
}

int main(int argc, char** argv)
{
    if (argc == 2  && std::strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    if (argc < 3) {
        print_help();
        return MISSING_ARGUMENT;
    }

    const std::string listening_ip = argv[1];
    const std::string listening_port = argv[2];

    if (listening_ip.empty() || listening_port.empty()) {
        std::cerr << "arguments must not be empty" << std::endl;
        return INVALID_ARGUMENT;
    }

    print_args(listening_ip, listening_port);

    auto server = Connectivity::Server::create(listening_ip, listening_port);
    if (server) {
        std::cout << "server created successfully" << std::endl;
    }

    return 0;
}
