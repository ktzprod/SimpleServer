#include <atomic>
#include <iostream>
#include <thread>
#include <regex>
#include <signal.h>
#include <string>
#include <vector>

#include "server.h"
#include "validator.h"

#define INVALID_ARGUMENT -1
#define MISSING_ARGUMENT -2
#define SERVER_FAILURE -3

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
    std::cout << "\t" << "SERVER_FAILURE(" << SERVER_FAILURE << ")" << std::endl;
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

int check_args(const std::string& listening_ip, const std::string& listening_port)
{
    if (listening_ip.empty() || listening_port.empty()) {
        std::cerr << "arguments must not be empty" << std::endl;
        return INVALID_ARGUMENT;
    }

    if (!Connectivity::Validator::is_valid_port(listening_port)) {
        std::cerr << "listening port is not valid" << std::endl;
        return INVALID_ARGUMENT;
    }

    if (!Connectivity::Validator::is_valid_ipv4(listening_ip)) {
        std::cerr << "listening ip is not a valid IPv4 address" << std::endl;
        return INVALID_ARGUMENT;
    }

    return 0;
}

static std::atomic_bool on_sig_int_flag = false;
static void on_signal_handler(int sig)
{
    if (sig == SIGINT) {
        on_sig_int_flag = true;
    }
}

using Server = std::unique_ptr<Connectivity::Server>;

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

    if (int result = check_args(listening_ip, listening_port); result < 0) {
        return result;
    }

    print_args(listening_ip, listening_port);

    struct sigaction signal_handler;
    signal_handler.sa_handler = &on_signal_handler;
    sigemptyset(&signal_handler.sa_mask);
    signal_handler.sa_flags = 0;
    sigaction(SIGINT, &signal_handler, NULL);

    Server server = Connectivity::Server::create(listening_ip, listening_port);
    if (!server) {
        std::cerr << "failed to create server with the given arguments" << std::endl;
        return SERVER_FAILURE;
    }

    std::cout << "server created successfully" << std::endl;
    server->run(std::chrono::milliseconds(1000), []{ return on_sig_int_flag.load(); });
    return 0;
}
